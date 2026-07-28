#include <units/ChemicalSiegeTank.h>

#include <algorithm>

#include <globals.h>

#include <Bullet.h>
#include <FileClasses/GFXManager.h>
#include <Command.h>
#include <Game.h>
#include <Map.h>
#include <Tile.h>
#include <players/Player.h>
#include <House.h>
#include <SoundPlayer.h>

ChemicalSiegeTank::ChemicalSiegeTank(House* newOwner) : EliteSiegeTank(newOwner) {
    initChemicalSiegeTank();
}

ChemicalSiegeTank::ChemicalSiegeTank(InputStream& stream) : EliteSiegeTank(stream) {
    initChemicalSiegeTank();
}

ChemicalSiegeTank::~ChemicalSiegeTank() = default;

void ChemicalSiegeTank::initChemicalSiegeTank() {
    itemID = Unit_ChemicalSiegeTank;
    gunGraphicID = ObjPic_ChemicalSiegeTankGunTornie;
    turretGraphic = pGFXManager->getObjPic(gunGraphicID, getOwner()->getHouseID());
}

bool ChemicalSiegeTank::isFriendly(const ObjectBase* object) const {
    return object != nullptr && object->getOwner() != nullptr && getOwner() != nullptr
        && object->getOwner()->getTeamID() == getOwner()->getTeamID();
}

bool ChemicalSiegeTank::isFriendlyDamagedUnit(const ObjectBase* object) const {
    const UnitBase* unit = dynamic_cast<const UnitBase*>(object);
    return unit != nullptr && unit != this && unit->isActive() && isFriendly(unit)
        && unit->getHealth() < unit->getMaxHealth();
}

bool ChemicalSiegeTank::canAttack(const ObjectBase* object) const {
    return EliteSiegeTank::canAttack(object);
}

bool ChemicalSiegeTank::canHeal(const ObjectBase* object) const {
    return isFriendlyDamagedUnit(object);
}
void ChemicalSiegeTank::handleActionClick(int xPos, int yPos) {
    if(respondable && currentGameMap->tileExists(xPos, yPos)) {
        Tile* tile = currentGameMap->getTile(xPos, yPos);
        if(tile->hasAnObject()) {
            ObjectBase* actionTarget = tile->getObject();
            if(canHeal(actionTarget)) {
                currentGame->getCommandManager().addCommand(Command(
                    pLocalPlayer->getPlayerID(), CMD_CHEMICAL_HEALOBJECT,
                    objectID, actionTarget->getObjectID()));
                return;
            }
        }
    }

    EliteSiegeTank::handleActionClick(xPos, yPos);
}

void ChemicalSiegeTank::doHealObject(Uint32 targetObjectID) {
    ObjectBase* pTarget = currentGame->getObjectManager().getObject(targetObjectID);
    if(!canHeal(pTarget)) {
        return;
    }

    healingTarget = true;
    doAttackObject(pTarget, true);
}

void ChemicalSiegeTank::targeting() {
    ObjectBase* currentTarget = target.getObjPointer();
    if(currentTarget != nullptr && isFriendly(currentTarget)) {
        if(!healingTarget) {
            releaseTarget();
        } else if(const ObjectBase* enemy = findTarget(); enemy != nullptr && !isFriendly(enemy)) {
            healingTarget = false;
            doAttackObject(enemy, false);
        } else if(!isFriendlyDamagedUnit(currentTarget)) {
            healingTarget = false;
            setForced(false);
            releaseTarget();
        }
    } else if(currentTarget != nullptr) {
        healingTarget = false;
    } else {
        healingTarget = false;
    }

    EliteSiegeTank::targeting();

    if(target || moving || forced || (destination.isValid() && destination != location)
       || attackMode == STOP || attackMode == RETREAT
       || attackMode == CARRYALLREQUESTED) {
        return;
    }

    const int healScanRange = attackMode == AREAGUARD
        ? std::max(getWeaponRange(), 10)
        : getWeaponRange();

    for(UnitBase* candidate : unitList) {
        if(!isFriendlyDamagedUnit(candidate)) {
            continue;
        }

        const Coord closestPoint = candidate->getClosestPoint(location);
        if(blockDistance(location, closestPoint) <= healScanRange) {
            healingTarget = true;
            doAttackObject(candidate, true);
            return;
        }
    }
}

bool ChemicalSiegeTank::attack() {
    ObjectBase* currentTarget = target.getObjPointer();
    if(currentTarget == nullptr || !isFriendly(currentTarget)) {
        return EliteSiegeTank::attack();
    }

    if(!healingTarget) {
        return false;
    }

    if(!isFriendlyDamagedUnit(currentTarget) || primaryWeaponTimer != 0
       || getCurrentAttackAngle() != targetAngle) {
        return false;
    }

    Coord centerPoint = getCenterPoint();
    Coord targetCenterPoint = currentTarget->getCenterPoint();
    const int healAmount = std::max(1, currentTarget->getMaxHealth() / 10);
    bulletList.push_back(new Bullet(objectID, &centerPoint, &targetCenterPoint,
                                    Bullet_ChemicalHeal, healAmount, false, currentTarget));
    lastFiredBulletType = Bullet_ChemicalHeal;
    soundPlayer->playSoundAt(Sound_Rocket, location);

    primaryWeaponTimer = getWeaponReloadTime() * 2;
    return true;
}

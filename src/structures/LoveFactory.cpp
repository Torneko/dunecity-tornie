#include <structures/LoveFactory.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <House.h>

LoveFactory::LoveFactory(House* newOwner) : StructureBase(newOwner) {
    LoveFactory::init();
    setHealth(getMaxHealth());
}

LoveFactory::LoveFactory(InputStream& stream) : StructureBase(stream) {
    LoveFactory::init();
}

void LoveFactory::init() {
    itemID = Structure_LoveFactory;
    owner->incrementStructures(itemID);

    structureSize.x = 2;
    structureSize.y = 3;
    graphicID = ObjPic_LoveFactory;
    graphic = pGFXManager->getObjPic(graphicID, getOwner()->getHouseID());

    numImagesX = 8;
    numImagesY = 1;
    firstAnimFrame = 2;
    lastAnimFrame = 7;
    curAnimFrame = 2;
    lastVisibleFrame = 2;
    animationCounter = 0;
}

LoveFactory::~LoveFactory() = default;
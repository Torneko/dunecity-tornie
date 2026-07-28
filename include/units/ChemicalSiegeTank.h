#ifndef CHEMICALSIEGETANK_H
#define CHEMICALSIEGETANK_H

#include <units/EliteSiegeTank.h>

class ChemicalSiegeTank final : public EliteSiegeTank
{
public:
    explicit ChemicalSiegeTank(House* newOwner);
    explicit ChemicalSiegeTank(InputStream& stream);
    ~ChemicalSiegeTank() override;

    void handleActionClick(int xPos, int yPos) override;

    bool canAttack(const ObjectBase* object) const override;
    bool canHeal(const ObjectBase* object) const;
    void doHealObject(Uint32 targetObjectID);

protected:
    bool attack() override;
    void targeting() override;

private:
    void initChemicalSiegeTank();
    bool isFriendly(const ObjectBase* object) const;
    bool isFriendlyDamagedUnit(const ObjectBase* object) const;
    bool healingTarget = false;
};

#endif // CHEMICALSIEGETANK_H

/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef CHEMICALSIEGETANK_H
#define CHEMICALSIEGETANK_H

#include <units/TankBase.h>

/// Chemical Siege Tank used by the Tornie/Jericho custom house.
class ChemicalSiegeTank final : public TankBase
{
public:
    explicit ChemicalSiegeTank(House* newOwner);
    explicit ChemicalSiegeTank(InputStream& stream);
    void init();
    virtual ~ChemicalSiegeTank();

    void blitToScreen() override;
    void destroy() override;
    void playAttackSound() override;
};

#endif // CHEMICALSIEGETANK_H

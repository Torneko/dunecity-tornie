#ifndef LOVEFACTORY_H
#define LOVEFACTORY_H

#include <structures/StructureBase.h>

/// Animated 2x3 Love Factory from the Tornie/Jericho content set.
class LoveFactory final : public StructureBase
{
public:
    explicit LoveFactory(House* newOwner);
    explicit LoveFactory(InputStream& stream);
    virtual ~LoveFactory();

private:
    void init();
};

#endif // LOVEFACTORY_H
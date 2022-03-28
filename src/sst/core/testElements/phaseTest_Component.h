// Copyright 2009-2021 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2021, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef SST_CORE_PHASETEST_COMPONENT_H
#define SST_CORE_PHASETEST_COMPONENT_H

#include "sst/core/component.h"
#include "sst/core/link.h"
#include "sst/core/rng/marsaglia.h"

namespace SST {
namespace PhaseTestComponent {

// These first two classes are just base classes to test ELI
// inheritance.  The definition of the ELI items are spread through 2
// component base classes to make sure they get inherited in the
// actual component that can be instanced.
class PhaseTestComponentBase : public SST::Component
{
public:
    SST_ELI_REGISTER_COMPONENT_BASE(SST::PhaseTestComponent::PhaseTestComponentBase)

    SST_ELI_DOCUMENT_PARAMS(
        { "workPerCycle", "Count of busy work to do during a clock tick.", NULL}
    )

    SST_ELI_DOCUMENT_STATISTICS(
        { "N", "events sent on N link", "counts", 1 }
    )

    SST_ELI_DOCUMENT_PORTS(
        {"Nlink", "Link to the PhaseTestComponent to the North", { "PhaseTestComponent.PhaseTestComponentEvent", "" } }
    )

    PhaseTestComponentBase(ComponentId_t id) : SST::Component(id) {}
    ~PhaseTestComponentBase() {}
};

class PhaseTestComponentBase2 : public PhaseTestComponentBase
{
public:
    SST_ELI_REGISTER_COMPONENT_DERIVED_BASE(
        SST::PhaseTestComponent::PhaseTestComponentBase2, SST::PhaseTestComponent::PhaseTestComponentBase)

    SST_ELI_DOCUMENT_PARAMS(
        { "commFreq",     "Approximate frequency of sending an event during a clock tick.", NULL},
    )

    SST_ELI_DOCUMENT_STATISTICS(
        { "S", "events sent on S link", "counts", 1 }
    )

    SST_ELI_DOCUMENT_PORTS(
        {"Slink", "Link to the PhaseTestComponent to the South", { "PhaseTestComponent.PhaseTestComponentEvent", "" } }
    )

    PhaseTestComponentBase2(ComponentId_t id) : PhaseTestComponentBase(id) {}
    ~PhaseTestComponentBase2() {}
};

class PhaseTestComponent : public PhaseTestComponentBase2
{
public:
    // REGISTER THIS COMPONENT INTO THE ELEMENT LIBRARY
    SST_ELI_REGISTER_COMPONENT(
        PhaseTestComponent,
        "coreTestElement",
        "PhaseTestComponent",
        SST_ELI_ELEMENT_VERSION(1,0,0),
        "CoreTest Test Component",
        COMPONENT_CATEGORY_PROCESSOR
    )

    SST_ELI_DOCUMENT_PARAMS(
        { "commSize",     "Size of communication to send.", "16"}
    )

    SST_ELI_DOCUMENT_STATISTICS(
        { "E", "events sent on E link", "counts", 1 },
        { "W", "events sent on W link", "counts", 1 }
    )

    SST_ELI_DOCUMENT_PORTS(
        {"Elink", "Link to the PhaseTestComponent to the East",  { "PhaseTestComponent.PhaseTestComponentEvent", "" } },
        {"Wlink", "Link to the PhaseTestComponent to the West",  { "PhaseTestComponent.PhaseTestComponentEvent", "" } }
    )

    // Optional since there is nothing to document
    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
    )

    PhaseTestComponent(SST::ComponentId_t id, SST::Params& params);
    ~PhaseTestComponent();

    void setup() {}
    void finish() { printf("Component Finished.\n"); }

private:
    PhaseTestComponent();                         // for serialization only
    PhaseTestComponent(const PhaseTestComponent&); // do not implement
    void operator=(const PhaseTestComponent&);    // do not implement

    void         handleEvent(SST::Event* ev);
    virtual bool clockTic(SST::Cycle_t);

    int workPerCycle;
    int commFreq;
    int commSize;
    int neighbor;

    SST::RNG::MarsagliaRNG*          rng;
    SST::Link*                       N;
    SST::Link*                       S;
    SST::Link*                       E;
    SST::Link*                       W;
    SST::Statistics::Statistic<int>* countN;
    SST::Statistics::Statistic<int>* countS;
    SST::Statistics::Statistic<int>* countE;
    SST::Statistics::Statistic<int>* countW;
};

} // namespace PhaseTestComponent
} // namespace SST

#endif // SST_CORE_CORETEST_COMPONENT_H

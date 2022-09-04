#include "AST/SourceCode.h"

#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("1st Test") {
    shared_ptr<SourceCode> sc = make_shared<SourceCode>();
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sc, "TestProcedure 1");
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sc, "TestProcedure 1");

    sc->addProcedure(procedure1);
    sc->addProcedure(procedure2);

    require(sc->getProcedures()[0] == procedure1);
    require(sc->getProcedures()[1] == procedure2);

    // Negative case for sanity
    require(sc->getProcedures()[1] != procedure1);

    require(procedure1->getParent() == sc);
    require(procedure2->getParent() == sc);
    require(sc->getParent() == nullptr);
}



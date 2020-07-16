/**
 * This file will test the functionality of the Signal class
 */

#include <exception>

#include "percemon/ast.hpp"
#include "percemon/fmt.hpp"
#include "percemon_tests.hpp"

using namespace percemon::ast;

TEST_CASE("AST nodes throw exceptions when constructed badly", "[ast][except]") {
  SECTION("Pins have correct syntax") {
    REQUIRE_NOTHROW(Pin{Var_f{"f1"}});
    REQUIRE_NOTHROW(Pin{Var_x{"x1"}});
    REQUIRE_NOTHROW(Pin{Var_x{"x"}, Var_f{"f1"}});
    REQUIRE_NOTHROW(Pin{{}, Var_f{"f1"}});
    REQUIRE_NOTHROW(Pin{Var_x{"x"}, {}});

    REQUIRE_THROWS_AS(Pin({}, {}), std::invalid_argument);
  }
}

TEST_CASE("AST nodes are printed correctly", "[ast][fmt]") {
  REQUIRE("C_TIME" == fmt::to_string(C_TIME{}));
  REQUIRE("C_FRAME" == fmt::to_string(C_FRAME{}));

  SECTION("Pinned variables") {
    REQUIRE("f_1" == fmt::to_string(Var_f{"1"}));
    REQUIRE("x_1" == fmt::to_string(Var_x{"1"}));

    REQUIRE("{x_1, f_1}" == fmt::to_string(Pin{Var_x{"1"}, Var_f{"1"}}));
  }

  SECTION("Objects and quantifiers over them") {
    REQUIRE("id_1" == fmt::to_string(Var_id{"1"}));
    REQUIRE("EXISTS {id_1, id_2, id_3}" == fmt::to_string(Exists{{"1"}, {"2"}, {"3"}}));
    REQUIRE("FORALL {id_1, id_2, id_3}" == fmt::to_string(Forall{{"1"}, {"2"}, {"3"}}));
  }
}

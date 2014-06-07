/**
 * @file stratified_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "gdlparser_test.hpp"

BOOST_AUTO_TEST_SUITE(StratTest);

using namespace std;
using namespace gdlparser;

/**
 * Check stratified negation.
 */
BOOST_AUTO_TEST_CASE(StratifiedNotTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/notunstrat.kif");
	if(kif.Parse()) MARK_FAIL;
	MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();

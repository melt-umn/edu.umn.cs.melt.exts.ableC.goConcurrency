grammar determinism;

import edu:umn:cs:melt:ableC:host ;

-- I don't think you're supposed to run copper_mda tests on
-- terminal-only grammars? Grammars are expected to have bridge
-- productions, and terminal-only grammars have zero productions
-- to be inferred as a bridge production.

--copper_mda testArrow(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:arrow;
--}

--copper_mda testCloseTerm(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:closeTerm;
--}

--copper_mda testAssignTerm(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:selectAssignTerm;
--}

--copper_mda testSelectTerm(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:selectTerm;
--}

--copper_mda testSpawnTerm(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:spawnTerm;
--}

--copper_mda testSelectExpr(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:selectExpr;
--}

--copper_mda testSelectCases(ablecParser) {
--  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:selectCases;
--}

copper_mda testSend(ablecParser) {
  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:send;
}

copper_mda testRecieve(ablecParser) {
  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:recieve;
}

copper_mda testSpawn(ablecParser) {
  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:spawn;
}

copper_mda testClose(ablecParser) {
  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:close;
}

copper_mda testSelect(ablecParser) {
  edu:umn:cs:melt:exts:ableC:goConcurrency:concretesyntax:select;
}
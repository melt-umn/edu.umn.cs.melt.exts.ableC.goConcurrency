grammar edu:umn:cs:melt:exts:ableC:goConcurrency:src:concretesyntax:arrow;

imports edu:umn:cs:melt:ableC:concretesyntax;
imports silver:langutil only ast; 

marking terminal Arrow_t '<-' lexer classes{Ckeyword};

closed nonterminal ArrowOp_c;
concrete production arrowOp
top::ArrowOp_c ::= '<-' 
{
} 

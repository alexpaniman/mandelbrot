divert(-1)

# quote(args) - convert args to single-quoted string
define(`quote', `ifelse(`$#', `0', `', ``$*'')')
# dquote(args) - convert args to quoted list of quoted strings
define(`dquote', ``$@'')
# dquote_elt(args) - convert args to list of double-quoted strings
define(`dquote_elt', `ifelse(`$#', `0', `', `$#', `1', ```$1''',
                             ```$1'',$0(shift($@))')')

# foreach(x, (item_1, item_2, ..., item_n), stmt)
#   parenthesized list, improved version
define(`foreach', `pushdef(`$1')_$0(`$1',
  (dquote(dquote_elt$2)), `$3')popdef(`$1')')
define(`_arg1', `$1')
define(`_foreach', `ifelse(`$2', `(`')', `',
  `define(`$1', _arg1$2)$3`'$0(`$1', (dquote(shift$2)), `$3')')')

# foreachq(x, `item_1, item_2, ..., item_n', stmt)
#   quoted
define(`foreachq',
`pushdef(`$1')_$0(`$1', `$3'ifelse(`$2', `', `',
  `, $2'))popdef(`$1')')
define(`_foreachq', `ifelse(`$#', `2', `',
  `define(`$1', `$3')$2`'$0(`$1', `$2'ifelse(`$#', `3', `',
    `, shift(shift(shift($@)))'))')')

define(`CAMEL_TO_SNAKE_CASE',
	`translit(
	    patsubst(`$1', `\([A-Z]\)', `_\1'), `A-Z', `a-z')')

divert(0)dnl

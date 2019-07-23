# Gambit - Binary Operation Specification


## Grammar

```
T_MINUS T_IDENTIFIER
T_PLUS T_IDENTIFIER
T_IDENTIFIER T_EQUAL <expression>
```

## Examples

```
======================

let($catch_phrase) = 'Bender is great'
catch_phrase = 'Bender is not great'

<<< compiles to >>>
var catch_phrase = match([$, (catch_phrase) => catch_phrase ])('Bender is great');
catch_phrase == 'Bender is not great'

======================
```
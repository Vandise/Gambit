# Gambit - Binary Operation Specification


## Grammar

```
T_MINUS T_IDENTIFIER
T_PLUS T_IDENTIFIER
T_IDENTIFIER | <expression> T_EQUAL <expression>
T_IDENTIFIER | <expression> && <expression>
T_IDENTIFIER | <expression> || <expression>

T_IDENTIFIER | <expression> [>, <, >=, <=] <expression>
```

## Examples

```
======================

let($catch_phrase) = 'Bender is great'
catch_phrase = 'Bender is not great'

<<< compiles to >>>
const catch_phrase = match([$, (catch_phrase) => catch_phrase ])('Bender is great');
catch_phrase == 'Bender is not great'

======================

10 = 11

<<< compiles to >>>

(10 == 11)

======================

1 || 0

<<< compiles to >>>

(1 || 0)

======================

1 >= 0

<<< compiles to >>>

(1 >= 0)


```
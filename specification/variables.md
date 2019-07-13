# Gambit - Variable Specification


## Grammar

```
let( [ T_PARAM ] T_IDENTIFIER [ T_COLON T_CONSTANT|T_LITERAL ]) = <expression>
```

## Examples

```
struct Person do
  name: null
  age: null
end

======================

let(T_IDENTIFIER) = <expression>

let(year) = 2015


<<< compiles to >>>

var year = 2015;

======================

let(T_PARAM T_IDENTIFIER) = <expression>

let($year) = 2015

<<< compiles to >>>

var year = match([$, (year) => year ])(2015);

======================

let(T_PARAM T_IDENTIFIER T_COLON T_CONSTANT) = <expression>

let($year:Number) = 2015

<<< compiles to >>>

var year = match([Number, (year) => year ])(2015);

```
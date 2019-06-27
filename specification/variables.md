# Gambit - Variable Specification


## Grammar

```
let(<arguments) = <literal>
```

## Example

```
struct Person do
  name: null
  age: null
end

let(greeting) = 'Hello world!'

let(bender) = %Person{ name: 'Bender', age: 20 }

let(name) = bender
# => 'Bender'

let(name, age) = bender
# => 'Bender'
# => 20

let(name: 'Bob', age: 20) = bender
# => Error

let(name: 'Bob', age) = bender
# => Error


```
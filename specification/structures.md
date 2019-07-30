# Gambit - Structures Specification


## Grammar

```
T_STRUCT T_CONSTANT T_DO
  <struct_props>
T_END

struct_props:
  T_IDENTIFIER T_COLON <factor>
  | struct_props T_NEWLINE

```

## Examples

```
======================

struct User do
  name: ''
  age: 25
end

<<< compiles to >>>

function User(name, age){
  this.name = name || "";
  this.age = age || 25;
};

======================

```
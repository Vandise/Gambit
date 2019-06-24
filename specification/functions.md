# Gambit - Functions Specification

TODO:

- Identify async function calls

## Grammar

```
[T_ASYNC] T_FUNC_DEFINITION T_IDENTIFIER ( <arguments> ) T_DO
  <statements>
T_END
```

Where `T_ASYNC` is optional and `T_IDENTIFIER` is not a keyword.

## Example

```
using(fetch) from Window

func greet(name: 'Bender') do
  Console::log('Kill all humans')
end

func greet(name: $) do
  Console::log('Hello ', name, ' !') |> IO::print
end

async func get_username() do
  fetch('/current_user') |> greet
end
```

## JS Output

```
import { fetch } from 'core/window';
import { match } from 'core/match';

const $ = match.parameter;
const _ = match.wildcard;

function greet_bender() {
  return console.log('Kill all humans');
}

function greet_any(name) {
  return console.log('Hello ' + name + ' !');
}

async function get_username_none() {
  let result;
  try {
    result = await fetch('/current_user');
  } catch (e) {
    result = e;
  }

  return greet(result);
}

function greet() {
  return match(
    [ $('Bender'), greet_bender ],
    [ $(String), greet_any ]
  );
}

function get_username() {
  return match(
    [ get_username_none ]
  );
}
```
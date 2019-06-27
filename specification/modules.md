# Gambit - Modules Specification

Modules are the core of Gambit and allow defined functions to be namespaced. Modules cannot be nested in one another and default to the global scope.

TODO:

- Possible nested module syntax

## Grammar

```
T_MODULE_DEFINITION T_IDENTIFIER T_DO T_NEWLINE
  <expressions>
T_END
```

Where `T_IDENTIFIER` is not a keyword and expressions includes constant definitions, function definitions, struct definitions, and dependency imports.

## Example

```
module MyModule do
	using(fetch) from Window
	
	DEFAULT_STATUS = 404
	
	struct HttpResponse do
		status: DEFAULT_STATUS
		response: null
	end
	
	async func get( url: $ ) do
		fetch(url)
	end

	# ...
end
```

## JS Output

```js
import { fetch } from 'core/window';
import { match } from 'core/match';

const $ = match.parameter;
const _ = match.wildcard;

export const DEFAULT_STATUS = 404;

export function HttpResponse() {
  this.status = DEFAULT_STATUS;
  this.response = null;
}

export async function get_any(url) {
  let result;
  try {
    result = await window.fetch(url);
  } catch (e) {
    result = e.toString();
  }
  return result;
}

export default {
  HttpResponse,

  get: match(
    [ $, get_any ]
  )
};
```

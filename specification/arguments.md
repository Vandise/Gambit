# Gambit - Arguments Specification

## Grammar

```
// values
( T_IDENTIFIER [T_COLON T_IDENTIFIER  T_COMMA ...] )

// structs
( % T_STRUCT { <values> } )
```

## Example

```
func as_http_response( response: String ) do
  %HttpResponse{ status: :failed, response: null }
end

func get_data( %HttpResponse{ status: :ok, response } ) do
  response |> JSON.parse
end
```

Where response: String will be a bound variable and passed as an argument

Where response will be a bound variable in get_data and the struct will not be available.

## JS Output

```js
import { match } from 'core/match';

function as_http_response_string(response) {
  const struct = new HttpResponse();
  struct.status = 'failed';
  struct.response = null;

  return struct;
}

function get_data_httpresponse_ok_response(response) {
  return JSON.parse(response);
}

function as_http_response() {
  return match(
    [ $(String), as_http_response_string ]
  );
}

function get_data() {
  return match(
    [ { status: 'ok', response: $, constructor: HttpResponse }, get_data_httpresponse_ok_response ]
  );
}

```
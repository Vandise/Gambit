# Gambit - Arguments Specification

## Grammar

```
// values
( T_IDENTIFIER [T_COLON T_IDENTIFIER T_COMMA ...] )

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

func http_response_ok(r = %HttpResponse{ status: :ok }) do
  // analyze http_response
  true
end

func http_response_ok(r = %HttpResponse{ status: :failure }) do
  // analyze http_response
  true
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

function http_response_ok_httpresponse_ok(http_response) {
  // analyze http_response
  return true;
}

function http_response_ok_httpresponse_failure(http_response) {
  // analyze http_response
  return false;
}

function as_http_response() {
  return match(
    [ $(String), as_http_response_string ]
  );
}

function http_response_ok() {
  return match(
    [ $({ status: 'ok', constructor: HttpResponse }), http_response_ok_httpresponse_ok ],
    [ $({ status: 'failure', constructor: HttpResponse }), http_response_ok_httpresponse_failure ]
  );
}

function get_data() {
  return match(
    [ { status: 'ok', response: $, constructor: HttpResponse }, get_data_httpresponse_ok_response ]
  );
}

```
import 'whatwg-fetch';
import { apiUrl } from './../config.json';

/**
 * Parses the JSON returned by a network request
 *
 * @param  {object} response A response from a network request
 *
 * @return {object}          The parsed JSON from the request
 */
function parseJSON(response) {
  if (response.status === 204 || response.status === 205) {
    return null;
  }
  return response.json();
}

/**
 * Checks if a network request came back fine, and throws an error if not
 *
 * @param  {object} response   A response from a network request
 *
 * @return {object|undefined} Returns either the response, or throws an error
 */
function checkStatus(response) {
  if (response.status >= 200 && response.status < 300) {
    return response;
  }

  if (response.status === 401) {
    const storedJwt = localStorage.setItem("bazar-jwt", null);
  }

  const error = new Error(response.statusText);
  error.response = response;
  throw error;
}

export default function request(path, options) {
  const storedJwt = localStorage.getItem("bazar-jwt");

  const extendedOptions = {
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${storedJwt}`,
    },
    ...options,
  };

  return fetch(`${apiUrl}${path}`, extendedOptions)
    .then(checkStatus)
    .then(parseJSON);
}

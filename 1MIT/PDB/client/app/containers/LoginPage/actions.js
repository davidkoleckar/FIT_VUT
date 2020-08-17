import {
  LOGIN,
  LOGIN_SUCCESS,
} from './constants';

export function login(credentials) {
  return {
    type: LOGIN,
    credentials,
  };
}

export function loginSuccess(token) {
  return {
    type: LOGIN_SUCCESS,
    payload: token,
  };
}
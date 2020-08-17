import {
  LOAD_USERS,
  LOAD_USERS_SUCCESS,
  CREATE_USER,
  CREATE_USER_SUCCESS,
  UPDATE_USER,
  UPDATE_USER_SUCCESS,
  DELETE_USER,
  DELETE_USER_SUCCESS,
} from './constants';

export function loadUsers() {
  return {
    type: LOAD_USERS,
  };
}

export function loadUsersSuccess(users) {
  return {
    type: LOAD_USERS_SUCCESS,
    payload: users,
  };
}

export function createUser(user) {
  return {
    type: CREATE_USER,
    payload: user,
  };
}

export function createUserSuccess(user) {
  return {
    type: CREATE_USER_SUCCESS,
    payload: user,
  };
}

export function updateUser(user) {
  return {
    type: UPDATE_USER,
    payload: user,
  };
}

export function updateUserSuccess(user) {
  return {
    type: UPDATE_USER_SUCCESS,
    payload: user,
  };
}

export function deleteUser(userId) {
  return {
    type: DELETE_USER,
    payload: userId,
  };
}

export function deleteUserSuccess(userId) {
  return {
    type: DELETE_USER_SUCCESS,
    payload: userId,
  };
}
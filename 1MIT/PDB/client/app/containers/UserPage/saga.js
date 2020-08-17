import { takeLatest, takeEvery, call, put, all } from 'redux-saga/effects';

import request from 'utils/request';

import { LOAD_USERS, CREATE_USER, UPDATE_USER, DELETE_USER } from './constants';
import { loadUsersSuccess, createUserSuccess, updateUserSuccess, deleteUserSuccess } from './actions';

export function* getUsers() {
  try {
    const users = yield call(request, 'users');
    yield put(loadUsersSuccess(users));
  } catch (err) {
    yield console.log(err);
  }
}

export function* postUser({ payload }) {
  try {
    const user = yield call(request, 'users', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    yield put(createUserSuccess(user));
  } catch (err) {
    yield console.log(err);
  }
}

export function* putUser({ payload }) {
  try {
    const user = yield call(request, 'users', {
      method: 'PUT',
      body: JSON.stringify(payload),
    });
    yield put(updateUserSuccess(user));
  } catch (err) {
    yield console.log(err);
  }
}

export function* deleteUser({ payload }) {
  const userId = payload;
  try {
    yield call(request, `users/${userId}`, {
      method: 'DELETE',
    });
    yield put(deleteUserSuccess(userId));
  } catch (err) {
    yield console.log(err);
  }
}

/**
 * Root saga manages watcher lifecycle
 */
export default function* rootSaga() {
  yield all([
    takeLatest(LOAD_USERS, getUsers),
    takeEvery(CREATE_USER, postUser),
    takeEvery(UPDATE_USER, putUser),
    takeEvery(DELETE_USER, deleteUser),
  ]);
}

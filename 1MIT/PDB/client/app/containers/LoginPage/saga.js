import { takeLatest, put, call } from 'redux-saga/effects';

import request from 'utils/request';

import { LOGIN } from './constants';
import { loginSuccess } from './actions';

export function* authenticate({ credentials }) {
  try {
    const payload = yield call(request, 'login', {
      method: 'POST',
      body: JSON.stringify(credentials),
    });
    yield put(loginSuccess(payload.token));
  } catch (err) {
    yield console.log(err);
  }

}

/**
 * Root saga manages watcher lifecycle
 */
export function* saga() {
  yield takeLatest(LOGIN, authenticate);
}

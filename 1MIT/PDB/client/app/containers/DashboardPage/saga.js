import { takeLatest, put, call, all } from 'redux-saga/effects';

import request from 'utils/request';

import { LOAD_CARS_EDITS } from './constants';
import {
  loadCarsEditsSuccess,
} from './actions';

export function* getVehiclesEdits({ payload }) {
  try {
    const cars = yield call(request, `vehicles/edited?from=${payload.from}&to=${payload.to}`);
    yield put(loadCarsEditsSuccess(cars));
  } catch (err) {
    yield console.log(err);
  }
}

/**
 * Root saga manages watcher lifecycle
 */
export default function* rootSaga() {
  yield all([
    takeLatest(LOAD_CARS_EDITS, getVehiclesEdits),
  ]);
}

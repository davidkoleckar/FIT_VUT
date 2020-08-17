import { takeLatest, put, takeEvery, call, all } from 'redux-saga/effects';

import request from 'utils/request';

import { LOAD_CARS, CREATE_CAR, UPDATE_CAR, DELETE_CAR, UPLOAD_CAR_PHOTO, LOAD_CAR_BY_ID, ROTATE_CAR_PHOTO } from './constants';
import {
  loadCarsSuccess,
  loadCarByIdSuccess,
  createCarSuccess,
  updateCarSuccess,
  uploadCarPhotoSuccess,
  deleteCarSuccess,
  rotateCarPhotoSuccess,
} from './actions';

export function* getVehicles({ payload = 'false'}) {
  try {
    const cars = yield call(request, `vehicles?sold=${payload}`);
    yield put(loadCarsSuccess(cars));
  } catch (err) {
    yield console.log(err);
  }
}

export function* getVehicleById({ payload }) {
  try {
    const car = yield call(request, `vehicles/${payload}`);
    car.prices = yield call(request, `vehicles/${payload}/prices`);
    car.similar = yield call(request, `image/similar/${payload}`);
    yield put(loadCarByIdSuccess(car));
  } catch (err) {
    yield console.log(err);
  }
}
export function* postVehicle({ payload }) {
  try {
    const car = yield call(request, 'vehicles', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    yield put(createCarSuccess(car));
  } catch (err) {
    yield console.log(err);
  }
}

export function* putVehicle({ payload }) {
  try {
    const user = yield call(request, 'vehicles', {
      method: 'PUT',
      body: JSON.stringify(payload),
    });
    yield put(updateCarSuccess(user));
  } catch (err) {
    yield console.log(err);
  }
}

export function* uploadVehiclePhoto({ payload }) {
  try {
    const image = yield call(request, `image/${payload.car.id}`, {
      method: 'POST',
      body: JSON.stringify({ data: payload.photo }),
    });
    yield put(uploadCarPhotoSuccess(payload.car, image));
  } catch (err) {
    yield console.log(err);
  }
}

export function* rotateImage({ payload }) {
  try {
    const image = yield call(request, `image/rotate/${payload}`);
    yield put(rotateCarPhotoSuccess(payload, image));
  } catch (err) {
    yield console.log(err);
  }
}

export function* deleteVehicle({ payload }) {
  const vehicleId = payload;
  try {
    yield call(request, `vehicles/${vehicleId}`, {
      method: 'DELETE',
    });
    yield put(deleteCarSuccess(vehicleId));
  } catch (err) {
    yield console.log(err);
  }
}

/**
 * Root saga manages watcher lifecycle
 */
export default function* rootSaga() {
  yield all([
    takeLatest(LOAD_CARS, getVehicles),
    takeLatest(LOAD_CAR_BY_ID, getVehicleById),
    takeEvery(ROTATE_CAR_PHOTO, rotateImage),
    takeEvery(CREATE_CAR, postVehicle),
    takeEvery(UPDATE_CAR, putVehicle),
    takeEvery(UPLOAD_CAR_PHOTO, uploadVehiclePhoto),
    takeEvery(DELETE_CAR, deleteVehicle),
  ]);
}

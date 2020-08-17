import { takeLatest, takeEvery, put, all, call } from 'redux-saga/effects';

import request from 'utils/request';

import {LOAD_AREAS, LOAD_EMPTY_AREAS, LOAD_DEFAULT_AREAS, CREATE_AREA, UPDATE_AREA, DELETE_AREA, LOAD_AREA_DETAILS, LOAD_AREAS_DISTANCE, LOAD_AREAS_UNION} from './constants';
import { loadAreasSuccess, loadEmptyAreasSuccess, loadDefaultAreasSuccess, createAreaSuccess, updateAreaSuccess, deleteAreaSuccess, loadAreaDetailsSuccess, loadAreasDistanceSuccess, loadAreasUnionSuccess } from './actions';

export function* getAreas() {
  try {
    const areas = yield call(request, 'area');
    yield put(loadAreasSuccess(areas));
  } catch (err) {
    console.log(err);
  }
}

export function* getEmptyAreas() {
  try {
    const cars = yield call(request, 'area/empty');
    yield put(loadEmptyAreasSuccess(cars));
  } catch (err) {
    console.log(err);
  }
}

export function* getAreaDetails({payload}) {
  try {
    const details = {};
    details.space = yield call(request, `area/space/${payload.id}`);
    details.length = yield call(request, `area/length/${payload.id}`);
    if (payload.typ === 'sekce') {
      details.parkingSpots = yield call(request, `area/parkingSpots/${payload.id}`);
      details.averagePrice = yield call(request, `area/averagePrice/${payload.id}`);
    }

    yield put(loadAreaDetailsSuccess(details));
  } catch (err) {
    console.log(err);
  }
}

export function* getDefaultAreas() {
  try {
    const cars = yield call(request, 'area/default');
    yield put(loadDefaultAreasSuccess(cars));
  } catch (err) {
    console.log(err);
  }
}

export function* postArea({payload}) {
  try {
    const area = yield call(request, 'area', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    yield put(createAreaSuccess(area));
  } catch (err) {
    console.log(err);
  }
}

export function* putArea({payload}) {
  try {
    const area = yield call(request, `area`, {
      method: 'PUT',
      body: JSON.stringify(payload),
    });
    yield put(updateAreaSuccess(area));
  } catch (err) {
    console.log(err);
  }
}

export function* deleteArea({payload}) {
  try {
    yield call(request, `area/${payload}`, {
      method: 'DELETE',
    });
    yield put(deleteAreaSuccess(payload));
  } catch (err) {
    console.log(err);
  }
}

export function* getAreasUnion({payload}) {
  try {
    const cars = yield call(request, `area/vehicles?idA=${payload[0]}&idB=${payload[1]}`);
    yield put(loadAreasUnionSuccess(cars));
  } catch (err) {
    console.log(err);
  }
}

export function* getAreasDistance({payload}) {
  try {
    const distance = yield call(request, `area/distance?idA=${payload[0]}&idB=${payload[1]}`);
    yield put(loadAreasDistanceSuccess(distance));
  } catch (err) {
    console.log(err);
  }
}

/**
 * Root saga manages watcher lifecycle
 */
export default function* rootSaga() {
  yield all([
    takeLatest(LOAD_AREAS, getAreas),
    takeLatest(LOAD_EMPTY_AREAS, getEmptyAreas),
    takeLatest(LOAD_DEFAULT_AREAS, getDefaultAreas),
    takeEvery(CREATE_AREA, postArea),
    takeEvery(UPDATE_AREA, putArea),
    takeEvery(DELETE_AREA, deleteArea),
    takeEvery(LOAD_AREA_DETAILS, getAreaDetails),
    takeEvery(LOAD_AREAS_DISTANCE, getAreasDistance),
    takeEvery(LOAD_AREAS_UNION, getAreasUnion),
  ]);
}

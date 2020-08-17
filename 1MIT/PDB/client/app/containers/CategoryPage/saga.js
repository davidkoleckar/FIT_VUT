import { takeLatest, takeEvery, call, put, all } from 'redux-saga/effects';

import request from 'utils/request';

import { LOAD_CATEGORIES, CREATE_CATEGORY, UPDATE_CATEGORY, DELETE_CATEGORY } from './constants';
import { loadCategoriesSuccess, createCategorySuccess, updateCategorySuccess, deleteCategorySuccess } from './actions';

export function* getCategories() {
  try {
    const categories = yield call(request, 'category');
    yield put(loadCategoriesSuccess(categories));
  } catch (err) {
    yield console.log(err);
  }
}

export function* postCategory({ payload }) {
  try {
    const category = yield call(request, 'category', {
      method: 'POST',
      body: JSON.stringify(payload),
    });
    yield put(createCategorySuccess(category));
  } catch (err) {
    yield console.log(err);
  }
}

export function* putCategory({ payload }) {
  try {
    const category = yield call(request, 'category', {
      method: 'PUT',
      body: JSON.stringify(payload),
    });
    yield put(updateCategorySuccess(category));
  } catch (err) {
    yield console.log(err);
  }
}

export function* deleteCategory({ payload }) {
  try {
    const categoryId = payload
    yield call(request, `category/${categoryId}`, {
      method: 'DELETE',
    });
    yield put(deleteCategorySuccess(categoryId));
  } catch (err) {
    yield console.log(err);
  }
}

/**
 * Root saga manages watcher lifecycle
 */
export default function* rootSaga() {
  yield all([
    takeLatest(LOAD_CATEGORIES, getCategories),
    takeEvery(CREATE_CATEGORY, postCategory),
    takeEvery(UPDATE_CATEGORY, putCategory),
    takeEvery(DELETE_CATEGORY, deleteCategory),
  ]);
}

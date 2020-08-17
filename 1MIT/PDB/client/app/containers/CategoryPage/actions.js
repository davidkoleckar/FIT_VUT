import {
  LOAD_CATEGORIES,
  LOAD_CATEGORIES_SUCCESS,
  CREATE_CATEGORY,
  CREATE_CATEGORY_SUCCESS,
  UPDATE_CATEGORY,
  UPDATE_CATEGORY_SUCCESS,
  DELETE_CATEGORY,
  DELETE_CATEGORY_SUCCESS,
} from './constants';

export function loadCategories() {
  return {
    type: LOAD_CATEGORIES,
  };
}

export function loadCategoriesSuccess(categories) {
  return {
    type: LOAD_CATEGORIES_SUCCESS,
    payload: categories,
  };
}

export function createCategory(category) {
  return {
    type: CREATE_CATEGORY,
    payload: category,
  };
}

export function createCategorySuccess(category) {
  return {
    type: CREATE_CATEGORY_SUCCESS,
    payload: category,
  };
}

export function updateCategory(category) {
  return {
    type: UPDATE_CATEGORY,
    payload: category
  };
}

export function updateCategorySuccess(category) {
  return {
    type: UPDATE_CATEGORY_SUCCESS,
    payload: category,
  };
}

export function deleteCategory(categoryId) {
  return {
    type: DELETE_CATEGORY,
    payload: categoryId,
  };
}

export function deleteCategorySuccess(categoryId) {
  return {
    type: DELETE_CATEGORY_SUCCESS,
    payload: categoryId,
  };
}
import { Map } from 'immutable';

import {
  LOAD_CATEGORIES, LOAD_CATEGORIES_SUCCESS,
  CREATE_CATEGORY, CREATE_CATEGORY_SUCCESS,
  UPDATE_CATEGORY, UPDATE_CATEGORY_SUCCESS,
  DELETE_CATEGORY, DELETE_CATEGORY_SUCCESS,
} from './constants';

const initialState = Map({
  categories: Map(),
  isLoading: false,
});

function categoriesReducer(state = initialState, action) {
  switch (action.type) {
    case LOAD_CATEGORIES:
      return state.set('isLoading', true);
    case LOAD_CATEGORIES_SUCCESS:
      return state
        .merge({
          categories: state.get('categories').withMutations((categoriesMap) =>
            action.payload.forEach(item => categoriesMap.set(item.id, item))
          ),
          isLoading: false
        });
    case CREATE_CATEGORY:
      return state.set('isLoading', true);
    case CREATE_CATEGORY_SUCCESS:
      return state
        .merge({
          categories: state.get('categories').withMutations((categoryMap) => categoryMap.set(action.payload.id, action.payload)),
          isLoading: false
        });
    case UPDATE_CATEGORY:
      return state.set('isLoading', true);
    case UPDATE_CATEGORY_SUCCESS:
      return state
        .merge({
          categories: state.get('categories').set(action.payload.id, action.payload),
          isLoading: false
        });
    case DELETE_CATEGORY:
      return state.set('isLoading', true);
    case DELETE_CATEGORY_SUCCESS:
      return state
        .merge({
          categories: state.get('categories').delete(action.payload),
          isLoading: false
        });
    default:
      return state;
  }
}

export default categoriesReducer;

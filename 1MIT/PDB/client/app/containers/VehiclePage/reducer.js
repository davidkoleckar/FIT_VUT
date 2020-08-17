import { Map } from 'immutable';

import {
  LOAD_CARS, LOAD_CARS_SUCCESS,
  LOAD_CAR_BY_ID, LOAD_CAR_BY_ID_SUCCESS,
  CREATE_CAR, CREATE_CAR_SUCCESS,
  UPDATE_CAR, UPDATE_CAR_SUCCESS,
  DELETE_CAR, DELETE_CAR_SUCCESS,
  UPLOAD_CAR_PHOTO, UPLOAD_CAR_PHOTO_SUCCESS,
  ROTATE_CAR_PHOTO, ROTATE_CAR_PHOTO_SUCCESS,
} from './constants';

import {
  LOAD_CARS_EDITS, LOAD_CARS_EDITS_SUCCESS
} from '../DashboardPage/constants';

const initialState = Map({
  cars: Map(),
  isLoading: false,
});

function carsReducer(state = initialState, action) {
  switch (action.type) {
    case LOAD_CARS:
      return state.set('isLoading', true);
    case LOAD_CARS_SUCCESS:
      return state
        .merge({
          cars: Map().withMutations((carsMap) =>
            action.payload.forEach && action.payload.forEach(item => carsMap.set(item.id, item))
          ),
          isLoading: false
        });
    case LOAD_CARS_EDITS:
      return state.set('isLoading', true);
    case LOAD_CARS_EDITS_SUCCESS:
      return state
        .merge({
          edits: Map().withMutations((carsMap) =>
            action.payload.forEach && action.payload.forEach(item => carsMap.set(item.id, item))
          ),
          isLoading: false
        });
    case LOAD_CAR_BY_ID:
      return state.set('isLoading', true);
    case LOAD_CAR_BY_ID_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').set(action.payload.id, action.payload),
          isLoading: false,
        });
    case CREATE_CAR:
      return state.set('isLoading', true);
    case CREATE_CAR_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').withMutations((carsMap) => carsMap.set(action.payload.id, action.payload)),
          isLoading: false
        });
    case UPDATE_CAR:
      return state.set('isLoading', true);
    case UPDATE_CAR_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').set(action.payload.id, { ...state.getIn(['cars', action.payload.id]), ...action.payload }),
          isLoading: false
        });
    case UPLOAD_CAR_PHOTO:
      return state.set('isLoading', true);
    case UPLOAD_CAR_PHOTO_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').set(action.payload.car.id, { ...state.getIn(['cars', action.payload.car.id]), obrazekData: action.payload.photo }),
          isLoading: false
        });
    case ROTATE_CAR_PHOTO:
      return state.set('isLoading', true);
    case ROTATE_CAR_PHOTO_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').set(action.carId, { ...state.getIn(['cars', action.carId]), obrazekData: action.payload.data }),
          isLoading: false
        });
    case DELETE_CAR:
      return state.set('isLoading', true);
    case DELETE_CAR_SUCCESS:
      return state
        .merge({
          cars: state.get('cars').delete(action.payload),
          isLoading: false
        });
    default:
      return state;
  }
}

export default carsReducer;

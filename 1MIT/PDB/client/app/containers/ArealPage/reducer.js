import { Map, List } from 'immutable';

import {
  LOAD_AREAS, LOAD_AREAS_SUCCESS,
  CREATE_AREA, CREATE_AREA_SUCCESS,
  UPDATE_AREA, UPDATE_AREA_SUCCESS,
  DELETE_AREA, DELETE_AREA_SUCCESS,
  LOAD_EMPTY_AREAS, LOAD_EMPTY_AREAS_SUCCESS,
  LOAD_AREA_DETAILS, LOAD_AREA_DETAILS_SUCCESS,
  LOAD_DEFAULT_AREAS, LOAD_DEFAULT_AREAS_SUCCESS,
  LOAD_AREAS_UNION, LOAD_AREAS_UNION_SUCCESS,
  LOAD_AREAS_DISTANCE, LOAD_AREAS_DISTANCE_SUCCESS,
  RESET_AREAS
} from './constants';

const initialState = Map({
  emptyAreas: List(),
  defaultAreas: Map(),
  selectedAreaDetails: null,
  selectedAreaDetailsIsLoading: false,
  areas: Map(),
  isLoading: false,
  selectedAreasDistance: null,
  selectedAreasDistanceIsLoading: false,
  selectedAreasUnion: null,
  selectedAreasUnionIsLoading: false,
});



function arealReducer(state = initialState, action) {
  switch (action.type) {
    case LOAD_AREAS:
      return state.set('isLoading', true);
    case LOAD_AREAS_SUCCESS:
      return state
        .merge({
          areas: Map().withMutations((areasMap) =>
            action.payload.forEach(item => areasMap.set(item.id, item))
          ),

          isLoading: false
        });
    case CREATE_AREA:
      return state.set('isLoading', true);
    case CREATE_AREA_SUCCESS:
      return state
        .merge({
          areas: state.get('areas').withMutations((areasMap) =>
            areasMap.set(action.payload.id, action.payload)
          ),
          isLoading: false
        });
    case UPDATE_AREA:
      return state.set('isLoading', true);
    case UPDATE_AREA_SUCCESS:
      return state
        .merge({
          areas: state.get('areas').withMutations((areasMap) =>
            areasMap.set(action.payload.id, action.payload)
          ),
          isLoading: false
        });
    case LOAD_DEFAULT_AREAS:
      return state.set('isLoading', true);
    case LOAD_DEFAULT_AREAS_SUCCESS:
      return state
        .merge({
          defaultAreas: Map().withMutations((areasMap) =>
            action.payload.forEach(item => areasMap.set(item.id, item))
          ),
          isLoading: false
        });
    case LOAD_AREA_DETAILS:
      return state
        .merge({
          selectedAreaDetails: null,
          selectedAreaDetailsIsLoading: true
        });
    case LOAD_AREA_DETAILS_SUCCESS:
      return state
        .merge({
          selectedAreaDetails: action.payload,
          selectedAreaDetailsIsLoading: false
        });

    case LOAD_AREAS_DISTANCE:
      return state
        .merge({
          selectedAreasDistance: null,
          selectedAreasUnion: null,
          selectedAreasDistanceIsLoading: true
        });
    case LOAD_AREAS_DISTANCE_SUCCESS:
      return state
        .merge({
          selectedAreaDetails: action.payload,
          selectedAreaDetailsIsLoading: false
        });

    case LOAD_AREAS_UNION:
      return state
        .merge({
          selectedAreasDistance: null,
          selectedAreasUnion: null,
          selectedAreasUnionIsLoading: true
        });
    case LOAD_AREAS_UNION_SUCCESS:
      return state
        .merge({
          selectedAreasUnion: action.payload,
          selectedAreasUnionIsLoading: false
        });

    case RESET_AREAS:
      return state
        .merge({
          selectedAreasDistance: null,
          selectedAreasUnion: null,
        });

    case LOAD_EMPTY_AREAS:
      return state.set('isLoading', true);
    case LOAD_EMPTY_AREAS_SUCCESS:
      return state
        .merge({
          emptyAreas: List(action.payload),
          isLoading: false
        });
    case DELETE_AREA:
      return state.set('isLoading', true);
    case DELETE_AREA_SUCCESS:
      return state
        .merge({
          areas: state.get('areas').delete(action.payload),
          isLoading: false
        });
    default:
      return state;
  }
}

export default arealReducer;

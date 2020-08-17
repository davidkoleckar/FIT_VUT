import { Map } from 'immutable';

import {
  LOAD_USERS, LOAD_USERS_SUCCESS,
  CREATE_USER, CREATE_USER_SUCCESS,
  UPDATE_USER, UPDATE_USER_SUCCESS,
  DELETE_USER, DELETE_USER_SUCCESS
} from './constants';

const initialState = Map({
  users: Map(),
  isLoading: false,
});

function usersReducer(state = initialState, action) {
  switch (action.type) {
    case LOAD_USERS:
      return state.set('isLoading', true);
    case LOAD_USERS_SUCCESS:
      return state
        .merge({
          users: state.get('users').withMutations((usersMap) =>
            action.payload.forEach(item => usersMap.set(item.id, item))
          ),
          isLoading: false
        });
    case CREATE_USER:
      return state.set('isLoading', true);
    case CREATE_USER_SUCCESS:
      return state
        .merge({
          users: state.get('users').withMutations((usersMap) => usersMap.set(action.payload.id, action.payload)),
          isLoading: false
        });
    case UPDATE_USER:
      return state.set('isLoading', true);
    case UPDATE_USER_SUCCESS:
      return state
        .merge({
          users: state.get('users').set(action.payload.id, action.payload),
          isLoading: false
        });
    case DELETE_USER:
      return state.set('isLoading', true);
    case DELETE_USER_SUCCESS:
      return state
        .merge({
          users: state.get('users').delete(action.payload),
          isLoading: false
        });
    default:
      return state;
  }
}

export default usersReducer;

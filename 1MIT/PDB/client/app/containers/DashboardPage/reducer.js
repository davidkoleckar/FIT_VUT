import { Map } from 'immutable';

import {
  LOAD_CARS_EDITS, LOAD_CARS_EDITS_SUCCESS,
} from './constants';

const initialState = Map({
  edits: Map(),
  isLoading: false,
});

function supplyPointReducer(state = initialState, action) {
  switch (action.type) {
    case LOAD_CARS_EDITS:
      return state.set('isLoading', true);
    case LOAD_CARS_EDITS_SUCCESS:
      return state
        .merge({
          edits: Map().withMutations((editsMap) =>
            action.payload.forEach && action.payload.forEach(item => editsMap.set(item.id, item))
          ),
          isLoading: false
        });
    default:
      return state;
  }
}

export default supplyPointReducer;

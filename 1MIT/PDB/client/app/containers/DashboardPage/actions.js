import moment from 'moment';
import {
  LOAD_CARS_EDITS,
  LOAD_CARS_EDITS_SUCCESS,
} from './constants';

export function loadCarsEdits(filter) {
  const payload = {
    from: filter.from + ' 00:00:00',
    to: filter.to + ' ' + moment().format('HH:mm:ss'),
  };
  return {
    type: LOAD_CARS_EDITS,
    payload: payload
  };
}

export function loadCarsEditsSuccess(cars) {
  return {
    type: LOAD_CARS_EDITS_SUCCESS,
    payload: cars,
  };
}
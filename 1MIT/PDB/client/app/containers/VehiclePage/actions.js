import moment from 'moment';

import {
  LOAD_CARS,
  LOAD_CARS_SUCCESS,
  LOAD_CAR_BY_ID,
  LOAD_CAR_BY_ID_SUCCESS,
  CREATE_CAR,
  CREATE_CAR_SUCCESS,
  UPDATE_CAR,
  UPDATE_CAR_SUCCESS,
  UPLOAD_CAR_PHOTO,
  UPLOAD_CAR_PHOTO_SUCCESS,
  DELETE_CAR,
  DELETE_CAR_SUCCESS,
  ROTATE_CAR_PHOTO,
  ROTATE_CAR_PHOTO_SUCCESS,
} from './constants';

export function loadCars(sold) {
  return {
    type: LOAD_CARS,
    payload: sold
  };
}

export function loadCarsSuccess(cars) {
  return {
    type: LOAD_CARS_SUCCESS,
    payload: cars,
  };
}

export function loadCarById(carId) {
  return {
    type: LOAD_CAR_BY_ID,
    payload: carId
  };
}

export function loadCarByIdSuccess(car) {
  return {
    type: LOAD_CAR_BY_ID_SUCCESS,
    payload: car
  };
}


export function createCar(car) {
  car.stk = moment(car.stk, 'MM/YYYY').format('YYYY-MM-DD HH:mm:ss');
  return {
    type: CREATE_CAR,
    payload: car,
  };
}

export function createCarSuccess(car) {
  return {
    type: CREATE_CAR_SUCCESS,
    payload: car,
  };
}

export function updateCar(car) {
  car.stk = moment(car.stk, 'MM/YYYY').format('YYYY-MM-DD HH:mm:ss');
  car.prices = null;
  car.similar = null;
  return {
    type: UPDATE_CAR,
    payload: car,
  };
}

export function updateCarSuccess(car) {
  return {
    type: UPDATE_CAR_SUCCESS,
    payload: car,
  };
}

export function uploadCarPhoto(car, photo) {
  return {
    type: UPLOAD_CAR_PHOTO,
    payload: {car, photo},
  };
}

export function uploadCarPhotoSuccess(car, photo) {
  return {
    type: UPLOAD_CAR_PHOTO_SUCCESS,
    payload: {car, photo},
  };
}

export function deleteCar(carId) {
  return {
    type: DELETE_CAR,
    payload: carId,
  };
}

export function deleteCarSuccess(carId) {
  return {
    type: DELETE_CAR_SUCCESS,
    payload: carId,
  };
}

export function rotateCarPhoto(carId) {
  return {
    type: ROTATE_CAR_PHOTO,
    payload: carId,
  };
}

export function rotateCarPhotoSuccess(carId, photo) {
  return {
    type: ROTATE_CAR_PHOTO_SUCCESS,
    payload: photo,
    carId,
  };
}
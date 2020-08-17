import {
  LOAD_AREAS,
  LOAD_AREAS_SUCCESS,
  LOAD_EMPTY_AREAS,
  LOAD_EMPTY_AREAS_SUCCESS,
  LOAD_DEFAULT_AREAS,
  LOAD_DEFAULT_AREAS_SUCCESS,
  LOAD_AREA_DETAILS,
  LOAD_AREA_DETAILS_SUCCESS,
  CREATE_AREA,
  CREATE_AREA_SUCCESS,
  UPDATE_AREA,
  UPDATE_AREA_SUCCESS,
  DELETE_AREA,
  DELETE_AREA_SUCCESS,
  LOAD_AREAS_DISTANCE,
  LOAD_AREAS_DISTANCE_SUCCESS,
  LOAD_AREAS_UNION,
  LOAD_AREAS_UNION_SUCCESS,
  RESET_AREAS,
} from './constants';


export function loadAreas() {
  return {
    type: LOAD_AREAS,
  };
}

export function loadAreasSuccess(areas) {
  return {
    type: LOAD_AREAS_SUCCESS,
    payload: areas,
  };
}

export function loadAreaDetails(area) {
  return {
    type: LOAD_AREA_DETAILS,
    payload: area,
  };
}

export function loadAreaDetailsSuccess(data) {
  return {
    type: LOAD_AREA_DETAILS_SUCCESS,
    payload: data,
  };
}

export function loadEmptyAreas() {
  return {
    type: LOAD_EMPTY_AREAS,
  };
}

export function loadEmptyAreasSuccess(areas) {
  return {
    type: LOAD_EMPTY_AREAS_SUCCESS,
    payload: areas,
  };
}

export function loadDefaultAreas() {
  return {
    type: LOAD_DEFAULT_AREAS,
  };
}

export function loadDefaultAreasSuccess(areas) {
  return {
    type: LOAD_DEFAULT_AREAS_SUCCESS,
    payload: areas,
  };
}

export function createArea(area) {
  if (area.typEntity === 'kruh') {
    const tvar = area.tvar;
    return {
      type: CREATE_AREA,
      payload: { ...area, tvar: [{
        x: tvar[0].x + (area.offsetX || 0),
        y: tvar[0].y + (area.offsetX || 0),
        },
        tvar[1]
      ]},
    };
  } else {
    area.tvar.forEach(bod => {
      bod.x = bod.x + (area.offsetX || 0);
      bod.y = bod.y + (area.offsetY || 0);
    });

    return {
      type: CREATE_AREA,
      payload: area,
    };
  }
}

export function createAreaSuccess(area) {
  return {
    type: CREATE_AREA_SUCCESS,
    payload: area,
  };
}

export function updateArea(area) {
  area.tvar.forEach(bod => {
    bod.x = bod.x + (area.offsetX || 0);
    bod.y = bod.y + (area.offsetY || 0);
  });

  return {
    type: UPDATE_AREA,
    payload: area,
  };
}

export function updateAreaSuccess(area) {
  return {
    type: UPDATE_AREA_SUCCESS,
    payload: area,
  };
}

export function deleteArea(areaId) {
  return {
    type: DELETE_AREA,
    payload: areaId,
  };
}

export function deleteAreaSuccess(areaId) {
  return {
    type: DELETE_AREA_SUCCESS,
    payload: areaId,
  };
}



export function loadAreasDistance(areasIds) {
  return {
    type: LOAD_AREAS_DISTANCE,
    payload: areasIds
  };
}

export function loadAreasDistanceSuccess(data) {
  return {
    type: LOAD_AREAS_DISTANCE_SUCCESS,
    payload: data,
  };
}
export function loadAreasUnion(areasIds) {
  return {
    type: LOAD_AREAS_UNION,
    payload: areasIds
  };
}

export function loadAreasUnionSuccess(areas) {
  return {
    type: LOAD_AREAS_UNION_SUCCESS,
    payload: areas,
  };
}

export function resetAreas() {
  return {
    type: RESET_AREAS,
  };
}
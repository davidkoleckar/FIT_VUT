import { fromJS } from 'immutable';
import jwtDecode from 'jwt-decode';

import { LOGIN_SUCCESS } from 'containers/LoginPage/constants';

let userInitialState = fromJS({
  jmeno: null,
  email: null,
  adresa: null,
  isAuthenticated: false,
});

const storedJwt = localStorage.getItem("bazar-jwt");

if (storedJwt) {
  try {
    const decoded = jwtDecode(storedJwt);
    userInitialState = userInitialState.merge({
      ...decoded,
      isAuthenticated: true,
    });
  } catch (err) {
    console.log(err);
    localStorage.setItem("bazar-jwt", null);
  }
}

/**
 * Merge route into the global application state
 */
function userReducer(state = userInitialState, action) {
  switch (action.type) {
    case LOGIN_SUCCESS:
      localStorage.setItem("bazar-jwt", action.payload);
      const decoded = jwtDecode(action.payload);
      return state.merge({
        ...decoded,
        isAuthenticated: true,
      });
    default:
      return state;
  }
}

export default userReducer;

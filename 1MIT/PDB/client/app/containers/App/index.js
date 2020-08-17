import React from 'react';
import { Switch, Route } from 'react-router-dom';
import { Helmet } from 'react-helmet';
import { connectedRouterRedirect } from 'redux-auth-wrapper/history4/redirect';
import locationHelperBuilder from 'redux-auth-wrapper/history4/locationHelper';

import LoginPage from 'containers/LoginPage/Loadable';
import AuthenticatedApp from 'containers/AuthenticatedApp/Loadable';
import NotFoundPage from 'containers/NotFoundPage/Loadable';

const userIsAuthenticated = connectedRouterRedirect({
  redirectPath: '/login',
  authenticatedSelector: (state) => state.getIn(['user', 'isAuthenticated']),
  wrapperDisplayName: 'UserIsAuthenticated',
});

const locationHelper = locationHelperBuilder({});

const userIsNotAuthenticated = connectedRouterRedirect({
  redirectPath: (state, ownProps) => locationHelper.getRedirectQueryParam(ownProps) || '/',
  allowRedirectBack: false,
  authenticatedSelector: (state) => !state.getIn(['user', 'isAuthenticated']),
  wrapperDisplayName: 'UserIsNotAuthenticated',
});

export default function App() {
    return (
        <div style={{height: '100%'}}>
            <Helmet
                defaultTitle="Správa autobazaru"
            >
            </Helmet>
            <Switch>
                <Route path="/login" component={userIsNotAuthenticated(LoginPage)} />
                <Route path="/" component={userIsAuthenticated(AuthenticatedApp)} />
                <Route path="" component={NotFoundPage} />
            </Switch>
        </div>
    );
}

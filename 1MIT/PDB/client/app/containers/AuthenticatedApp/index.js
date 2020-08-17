import React from 'react';
import { Switch, Route } from 'react-router-dom';
import { compose } from 'redux';
import { connect } from 'react-redux';

import { Callout } from 'office-ui-fabric-react/lib/Callout';
import { Persona } from 'office-ui-fabric-react/lib/Persona';

import App from 'components/App';
import Content from 'components/Content';
import Header from 'components/Header';
import Nav from 'components/Nav';

import DashboardPage from 'containers/DashboardPage/Loadable';
import VehiclePage from 'containers/VehiclePage/Loadable';
import CategoryPage from 'containers/CategoryPage/Loadable';
import ArealPage from 'containers/ArealPage/Loadable';
import UserPage from 'containers/UserPage/Loadable';

export class AuthenticatedApp extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function
  constructor(props) {
    super(props);

    this.onClickUserPersona = this.onClickUserPersona.bind(this);
    this.onDismissUserPersona = this.onDismissUserPersona.bind(this);

    this.state = {
      userMenu: {
        isOpen: false,
      }
    }
  }

  onClickUserPersona() {
    this.setState({ userMenu: { isOpen: !this.state.userMenu.isOpen } });
  }

  onDismissUserPersona() {
    this.setState({ userMenu: { isOpen: false } });
  }

  render() {
    const { user } = this.props;

    return (
      <App>
        <Header>
          <div ref={(persona) => { this.userPersona = persona } }>
            <Persona
              onClick={this.onClickUserPersona}
              onDismiss={ this.onDismissUserPersona }
              imageInitials="TT"
              primaryText={user.get('jmeno')}
              secondaryText={user.get('sub')}
            />
          </div>
          { this.state.userMenu.isOpen
            && (
              <Callout
                className='p-3'
                gapSpace={ -5 }
                target={ this.userPersona }
              >
                <h5>
                  Supr nabidka
                </h5>
                <div>
                  <div>
                    <button onClick={this.props.onLogout}>Odhlasit se</button>
                  </div>
                </div>
              </Callout>
            )
          }
        </Header>
        <Content>
          <Nav />
          <div className="w-100" style={{ overflowY: "auto" }}>
            <Switch>
              <Route path="/" exact component={DashboardPage}  />
              <Route path="/vozidla" component={VehiclePage}  />
              <Route path="/areal" component={ArealPage}  />
              <Route path="/kategorie" component={CategoryPage}  />
              <Route path="/uzivatele" component={UserPage} />
            </Switch>
          </div>
        </Content>
      </App>
    );
  }
}

function mapStateToProps(state) {
  return {
    user: state.get('user'),
  }
}

function mapDispatchToProps(dispatch) {
  return {
    onLogout: () => dispatch(logout()),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);

export default compose(
  withConnect,
)(AuthenticatedApp);

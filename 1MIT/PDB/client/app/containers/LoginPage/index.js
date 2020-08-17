import React from 'react';
import PropTypes from 'prop-types';

import { Helmet } from 'react-helmet';
import { connect } from 'react-redux';
import { compose } from 'redux';
import injectSaga from 'utils/injectSaga';

import App from 'components/App';
import Content from 'components/Content';
import Header from 'components/Header';
import H1 from 'components/H1';

import { saga } from './saga';
import { login } from './actions';
import Form from './Form';


export class LoginPage extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);

    this.onChangeFormField = this.onChangeFormField.bind(this);
    this.onSubmitForm = this.onSubmitForm.bind(this);

    this.state = {
      form: {
        email: '',
        heslo: '',
      },
    };
  }

  onChangeFormField(update) {
    this.setState({
      form: { ...this.state.form, ...update },
    });
  }

  onSubmitForm(e) {
    e.preventDefault();
    this.props.onLogin(this.state.form);
  }

  render() {
    return (
      <App>
        <Helmet>
          <title>Login</title>
        </Helmet>
        <Header />
        <Content className="align-items-center justify-content-center">
          <div className="col-4">
            <H1 className="mb-3">Správa autobazaru</H1>
            <Form
              onSubmit={this.onSubmitForm}
              onChange={this.onChangeFormField}
              { ...this.state.form } />
          </div>
        </Content>
      </App>
    );
  }
}

LoginPage.propTypes = {
  onLogin: PropTypes.func.isRequired,
};

function mapDispatchToProps(dispatch) {
  return {
    onLogin: (credentials) => dispatch(login(credentials)),
  };
}

const withConnect = connect(null, mapDispatchToProps);

const withSaga = injectSaga({ key: 'login', saga });

export default compose(
  withSaga,
  withConnect,
)(LoginPage);

import React from 'react';
import PropTypes from 'prop-types';

import { connect } from 'react-redux';
import { compose } from 'redux';
import injectSaga from 'utils/injectSaga';
import injectReducer from 'utils/injectReducer';

import { CompoundButton } from 'office-ui-fabric-react/lib/Button';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';

import saga from './saga';
import { loadUsers, createUser, updateUser, deleteUser } from './actions';
import reducer from './reducer';
import Table from './Table';
import AddPanel from './AddPanel';

class UserPage extends React.Component { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);

    this._onShowPanel = this._onShowPanel.bind(this);
    this._onClosePanel = this._onClosePanel.bind(this);
    this._onCreateUser = this._onCreateUser.bind(this);

    this.state = {
      showPanel: false,
    };
  }

  componentDidMount() {
    this.props.loadUsers();
  }

  _onCreateUser(data) {
    this.props.onCreateUser(data);
    this._onClosePanel();
  }

  _onShowPanel() {
    this.setState({ showPanel: true });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }

  render() {
    const { users, isLoading, onUpdateUser, onDeleteUser } = this.props;

    return (
      <div>

        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Uživatelé</h1>
            </div>

            <div className="col-auto">
              <CompoundButton
                primary={ true }
                onClick={this._onShowPanel}
                description={<span>Otevře průvodce vytvořením nového uživatele.</span>}
              >
                Přidat uživatele
              </CompoundButton>
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          {users && users.size
            ? (
              <div>
                <Table items={users} onUpdateUser={onUpdateUser} onDeleteUser={onDeleteUser} />
                {isLoading && <OpacityLoader />}
              </div>
            )
            : isLoading ? <SpinnerLoader /> : <div>No results yet</div>
          }
        </div>
        <AddPanel
          header="Přidat uživatele"
          form={{}}
          onClosePanel={this._onClosePanel}
          onSubmit={this._onCreateUser}
          show={this.state.showPanel}
        />
      </div>

    );
  }
}

UserPage.propTypes = {
  loadUsers: PropTypes.func.isRequired,
};

function mapStateToProps(state) {
  return {
    users: state.getIn(['users', 'users']),
    isLoading: state.getIn(['users', 'isLoading'])
  };
}

function mapDispatchToProps(dispatch) {
  return {
    loadUsers: () => dispatch(loadUsers()),
    onCreateUser: (user) => dispatch(createUser(user)),
    onUpdateUser: (user) => dispatch(updateUser(user)),
    onDeleteUser: (user) => dispatch(deleteUser(user.id)),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);
const withSaga = injectSaga({ key: 'users', saga });
const withReducer = injectReducer({ key: 'users', reducer });

export default compose(
  withSaga,
  withConnect,
  withReducer
)(UserPage);
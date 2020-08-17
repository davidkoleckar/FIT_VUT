import React from 'react';
import { CheckboxVisibility } from 'office-ui-fabric-react/lib/DetailsList';
import { Persona } from 'office-ui-fabric-react/lib/Persona';
import { ActionButton } from 'office-ui-fabric-react/lib/Button';

import Table from 'components/Table';

import EditPanel from './EditPanel';

const getColumnsConfig = (props, openEditPanel) => [
  {
    minWidth: 220,
    name: "Kontakt",
    onRender: user => (
      <Persona
        imageInitials={user.jmeno.split(' ').map(s => s[0]).join('')}
        primaryText={user.jmeno}
        secondaryText={user.email}
      />
    ),
  },
  {
    minWidth: 220,
    name: "Adresa",
    fieldName: 'adresa',
  },
  {
    minWidth: 220,
    onRender: item => <div>
      <ActionButton
        onClick={() => openEditPanel(item)}
        iconProps={ { iconName: 'Edit' } }
      >
        Upravit
      </ActionButton>
      <ActionButton
        onClick={() => props.onDeleteUser(item)}
        iconProps={ { iconName: 'Delete', style: { color: 'red' } } }
      >
        Smazat
      </ActionButton>
    </div>,
  },

];

class UsersTable extends React.Component {

  constructor(props) {
    super(props);

    this._onShowPanel = this._onShowPanel.bind(this);
    this._onClosePanel = this._onClosePanel.bind(this);
    this._onUpdateUser = this._onUpdateUser.bind(this);

    this.state = {
      selectedUserId: null,
    }
  }

  _onUpdateUser(data) {
    this.props.onUpdateUser(data);
    this._onClosePanel();
  }

  _onShowPanel(user) {
    this.setState({ showPanel: true, selectedUserId: user.id });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }


  render() {
    const { items } = this.props;

    return (
      <div>
        <Table
          items={items.toArray()}
          checkboxVisibility={CheckboxVisibility.hidden}
          columns={ getColumnsConfig(this.props, this._onShowPanel) }
        />
        {this.state.selectedUserId && <EditPanel
          header="Upravit uživatele"
          form={items.get(this.state.selectedUserId) || {}}
          onClosePanel={this._onClosePanel}
          onSubmit={this._onUpdateUser}
          show={this.state.showPanel}
        />
        }
      </div>
    );
  }
}

export default UsersTable;
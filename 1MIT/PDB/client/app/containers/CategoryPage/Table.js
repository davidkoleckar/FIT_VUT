import React from 'react';

import { ActionButton } from 'office-ui-fabric-react/lib/Button';
import { CheckboxVisibility } from 'office-ui-fabric-react/lib/DetailsList';

import Table from 'components/Table';

import EditPanel from './EditPanel';

const getColumnsConfig = (props, openEditPanel) => [
  {
    name: 'Název',
    fieldName: 'nazev',
  },
  {
    minWidth: 220,
    onRender: item => <div>
      <ActionButton
        onClick={() => openEditPanel(item.id)}
        iconProps={ { iconName: 'Edit' } }
      >
        Upravit
      </ActionButton>
      <ActionButton
        onClick={() => props.onDeleteCategory(item)}
        iconProps={ { iconName: 'Delete', style: { color: 'red' } } }
      >
        Smazat
      </ActionButton>
    </div>,
  },
];

class CategoriesTable extends React.PureComponent {

    constructor(props) {
      super(props);

      this._onShowPanel = this._onShowPanel.bind(this);
      this._onClosePanel = this._onClosePanel.bind(this);
      this._onUpdateCategory = this._onUpdateCategory.bind(this);

      this.state = {
        selectedCategoryId: null,
      }
    }

  _onUpdateCategory(data) {
    this.props.onUpdateCategory(data);
    this._onClosePanel();
  }

  _onShowPanel(userId) {
    this.setState({ showPanel: true, selectedCategoryId: userId });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }

    render() {
        const { items } = this.props;

        return (
          <div>
            <Table
                items={ items.toArray() }
                checkboxVisibility={CheckboxVisibility.hidden}
                columns={ getColumnsConfig(this.props, this._onShowPanel) }
            />
            {this.state.selectedCategoryId && <EditPanel
              header="Upravit kategorii"
              form={items.get(this.state.selectedCategoryId) || {}}
              onClosePanel={this._onClosePanel}
              onSubmit={this._onUpdateCategory}
              show={this.state.showPanel}
            />
            }
          </div>
        );
    }
}

export default CategoriesTable;
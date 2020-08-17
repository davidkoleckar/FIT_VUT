import React from 'react';
import moment from 'moment';
import { Link } from 'react-router-dom';

import { CheckboxVisibility } from 'office-ui-fabric-react/lib/DetailsList';
import { ActionButton } from 'office-ui-fabric-react/lib/Button';

import Table from 'components/Table';
import Img from 'components/Img';

import EditPanel from './EditPanel';

const getColumnsConfig = (props, openEditPanel) => [
  {
    name: "Fotka",
    onRender: item => <Img src={ item.obrazekData ? `data:image/jpeg;base64,${item.obrazekData}` : "https://placebear.com/100/100" } />,
    maxWidth: 160,
  },
  {
    name: 'Značka',
    fieldName: 'znacka',
  },
  {
    name: 'Model',
    fieldName: 'model',
  },
  {
    name: 'Kategorie',
    onRender: item =>
      <div>{props.categories.get(item.kategorie).nazev}</div>,
  },
  {
    name: 'Stav tachometru',
    fieldName: 'kilometry',
    onRender: item =>
      <div>{item.kilometry} km</div>,
  },
  {
    name: 'Informace',
    onRender: item =>
      <div>
        Rok Vyroby: {item.rok}<br/>
        STK: {moment(item.stk).format('MM/YYYY')}<br/>
      </div>,
  },
  {
    minWidth: 260,
    onRender: item => <div>
      <ActionButton
        iconProps={ { iconName: 'Car' } }
      >
        <Link to={`/vozidla/${item.id}`}>Detail</Link>
      </ActionButton>
      <ActionButton
        onClick={() => openEditPanel(item)}
        iconProps={ { iconName: 'Edit' } }
      >
        Upravit
      </ActionButton>
      <ActionButton
        onClick={() => props.onDeleteCar(item)}
        iconProps={ { iconName: 'Delete', style: { color: 'red' } } }
      >
        Prodat
      </ActionButton>
    </div>,
  },

];

class CarsTable extends React.PureComponent {

  constructor(props) {
    super(props);

    this._onShowPanel = this._onShowPanel.bind(this);
    this._onClosePanel = this._onClosePanel.bind(this);
    this._onUpdateCar = this._onUpdateCar.bind(this);
    this._onUploadPhoto = this._onUploadPhoto.bind(this);

    this.state = {
      selectedCarId: null,
    }
  }

  _onUpdateCar(data) {
    this.props.onUpdateCar(data);
    this._onClosePanel();
  }

  _onUploadPhoto(carId, data) {
    this.props.onUploadCarPhoto(carId, data);
  }

  _onShowPanel(car) {
    this.setState({ showPanel: true, selectedCarId: car.id });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }

    render() {
        const { items, categories, emptyAreas } = this.props;

        return (
          <div>
            <Table
                items={ items.toArray() }
                checkboxVisibility={CheckboxVisibility.hidden}
                columns={ getColumnsConfig(this.props, this._onShowPanel) }
            />
            {this.state.selectedCarId && <EditPanel
              key={`${this.state.selectedCarId}-${Date.now()}`}
              header="Upravit vozidlo"
              form={{ ...items.get(this.state.selectedCarId), stk: moment(items.get(this.state.selectedCarId).stk).format('MM/YYYY')} || {}}
              onClosePanel={this._onClosePanel}
              onUploadPhoto={this._onUploadPhoto}
              onSubmit={this._onUpdateCar}
              show={this.state.showPanel}
              categories={categories}
              emptyAreas={emptyAreas}
            />}
          </div>
        );
    }
}

export default CarsTable;
import React from 'react';
import { compose } from 'redux';
import { connect } from 'react-redux';

import { CompoundButton } from 'office-ui-fabric-react/lib/Button';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';
import SoldToggle from 'components/Vehicle/SoldToggle';

import Table from './Table';
import AddPanel from './AddPanel';

import { loadCars, createCar, updateCar, uploadCarPhoto, deleteCar } from './actions';
import { loadCategories } from '../CategoryPage/actions';
import { loadEmptyAreas, loadAreas } from '../ArealPage/actions';

class List extends React.Component { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);

    this._onShowPanel = this._onShowPanel.bind(this);
    this._onClosePanel = this._onClosePanel.bind(this);
    this._onCreateCar = this._onCreateCar.bind(this);
    this._onLoadCars = this._onLoadCars.bind(this);
    this._renderSoldSwitch = this._renderSoldSwitch.bind(this);

    this.state = {
      showPanel: false,
    };
  }

  componentDidMount() {
    this.props.loadCars();

    if (!this.props.categories.size) {
      this.props.loadCategories()
    }
    if (!this.props.emptyAreas.size) {
      this.props.loadEmptyAreas()
    }
  }

  _onLoadCars(sold) {
    this.props.loadCars((!sold).toString());
  }

  _onCreateCar(data) {
    this.props.onCreateCar(data);
    this._onClosePanel();
  }

  _onShowPanel() {
    this.setState({ showPanel: true });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }

  _renderSoldSwitch() {
    return (
      <SoldToggle
        defaultChecked={ true }
        onChanged={this._onLoadCars}
        label='Zobrazit'
        offText='Prodané'
        onText='Aktivní'
      />
    );
  }

  render() {

    const { isLoading, cars, categories, emptyAreas, onUpdateCar, onUploadCarPhoto, onDeleteCar } = this.props;

    return (
      <div>

        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Vozidla</h1>
            </div>
            <div className="col-auto">
              <CompoundButton
                primary={ true }
                onClick={this._onShowPanel}
                description={<span>Otevře průvodce přidáním nového vozidla.</span>}
              >
                Přidat vozidlo
              </CompoundButton>
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          <CommandBar
            isSearchBoxVisible={ false }
            items={ [{
                key: 'sold',
                onRender: this._renderSoldSwitch,
                name: 'Prodane',
              },
            ] }
          />
          {cars && cars.size
            ? (
              <div>
                <Table
                  items={cars}
                  categories={categories}
                  emptyAreas={emptyAreas}
                  onUpdateCar={onUpdateCar}
                  onUploadCarPhoto={onUploadCarPhoto}
                  onDeleteCar={onDeleteCar}
                />
                {isLoading && <OpacityLoader />}
              </div>
            )
            : isLoading ? <SpinnerLoader /> : <div>No results yet</div>
          }
        </div>
        <AddPanel
          header="Přidat vozidlo"
          form={{}}
          categories={categories}
          emptyAreas={emptyAreas}
          onClosePanel={this._onClosePanel}
          onSubmit={this._onCreateCar}
          show={this.state.showPanel}
        />
      </div>
    );
  }
}


function mapStateToProps(state) {
  return {
    cars: state.getIn(['car', 'cars']),
    isLoading: state.getIn(['car', 'isLoading']),
    categories: state.getIn(['category', 'categories']),
    emptyAreas: state.getIn(['areal', 'emptyAreas']),
  }
}

function mapDispatchToProps(dispatch) {
  return {
    loadCars: (sold) => dispatch(loadCars(sold)),
    onCreateCar: (car) => dispatch(createCar(car)),
    onUpdateCar: (car) => dispatch(updateCar(car)),
    onUploadCarPhoto: (car, photo) => dispatch(uploadCarPhoto(car, photo)),
    onDeleteCar: (car) => dispatch(deleteCar(car.id)),
    loadCategories: () => dispatch(loadCategories()),
    loadEmptyAreas: () => dispatch(loadEmptyAreas()),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);

export default compose(
  withConnect,
)(List);
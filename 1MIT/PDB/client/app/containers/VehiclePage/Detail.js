import React from 'react';
import moment from 'moment';
import { compose } from 'redux';
import { connect } from 'react-redux';
import { Link } from 'react-router-dom';

import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';
import Img from 'components/Img';

import { loadCarById, updateCar, rotateCarPhoto, uploadCarPhoto } from './actions';
import { loadCategories } from '../CategoryPage/actions';
import { loadEmptyAreas, loadAreas } from '../ArealPage/actions';

import EditForm from './EditForm';

class Detail extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);
    this._parseFile = this._parseFile.bind(this);
    this._onUpdateCar = this._onUpdateCar.bind(this);
  }

  _onUpdateCar(data) {
    this.props.onUpdateCar(data);
    if (this.state.photo) {
      this.props.onUploadPhoto(this.state.form, this.state.photo);
    }
  }

  _parseFile(e) {
    const file = e.target.files[0];
    const reader = new FileReader();
    reader.onloadend = () => {
      this.setState({
        photo: reader.result.replace('data:image/jpeg;base64,', ''),
      });
    };
    reader.readAsDataURL(file);
  }

  componentDidMount() {
    const { match } = this.props;

    this.props.loadCarById(match.params.id);

    if (!this.props.categories.size) {
      this.props.loadCategories()
    }
    if (!this.props.emptyAreas.size) {
      this.props.loadEmptyAreas()
    }
  }

  render() {
    const { onRotateCarPhoto, match, cars, categories, emptyAreas } = this.props;
    const car = cars.get(match.params.id * 1);

    return (
      <div>

        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Detail vozidla {car && <span> - {car.znacka}, {car.model}</span>}</h1>
            </div>
            <div className="col-auto">
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          {car && (
           <div className="row">
            <div className="col-4">
              <h4>Fotka vozidla</h4>
              {car.obrazekData ? <div>
              <CommandBar
                isSearchBoxVisible={ false }
                items={ [
                  {
                    key: 'turnLeft',
                    name: 'Otočit',
                    icon: 'ReplyMirrored',
                    onClick: () => onRotateCarPhoto(car.id)
                  },
                ] }
              />
              <Img src={`data:image/jpeg;base64,${car.obrazekData}`} />
              <h4>Podobna vozidla</h4>
              <div className="row">
              {car.similar && car.similar.map(item => (
                <div className="col-4">
                  <Link to={`/vehicles/${item.id}`}>
                    <Img src={`data:image/jpeg;base64,${item.data}`} />
                  </Link>
                </div>
              ))}
              </div>
              </div> :
                <div className="row mb-3">
                  <div className="col-3">Fotka</div>
                  <div className="col-8">
                    <input
                      type="file"
                      onChange={this._parseFile}
                    />
                  </div>
                </div>
              }
          </div>
            <div className="col-7">
              <h4>Údaje vozidla</h4>

              <EditForm
                form={{ ...car, stk: moment(car.stk).format('MM/YYYY')} || {}}
                categories={categories}
                emptyAreas={emptyAreas}
                onSubmit={this._onUpdateCar}
              />

              <h4>Změny ceny</h4>
              <table className="table table-sm">
                <thead><tr><td>Zmenil</td><td>Částka</td><td>Platnost od</td><td>Platnost do</td></tr></thead>
                <tbody>
                  {car.prices && car.prices.map(priceRecord => <tr key={priceRecord.cena+priceRecord.validFrom+priceRecord.validTo}>
                    <td>{priceRecord.jmeno}</td>
                    <td>{priceRecord.cena} Kč</td>
                    <td>{priceRecord.validFrom && moment(priceRecord.validFrom).format('YYYY-MM-DD')}</td>
                    <td>{priceRecord.validTo && moment(priceRecord.validTo).format('YYYY-MM-DD')}</td>
                  </tr>)}
                </tbody>
              </table>
            </div>
          </div>
          )}
        </div>
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
    loadCarById: (carId) => dispatch(loadCarById(carId)),
    onUpdateCar: (car) => dispatch(updateCar(car)),
    onRotateCarPhoto: (carId) => dispatch(rotateCarPhoto(carId)),
    onUploadCarPhoto: (car, photo) => dispatch(uploadCarPhoto(car, photo)),
    loadCategories: () => dispatch(loadCategories()),
    loadEmptyAreas: () => dispatch(loadEmptyAreas()),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);

export default compose(
  withConnect,
)(Detail);

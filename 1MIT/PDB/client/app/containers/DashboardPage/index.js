import React from 'react';
import moment from 'moment';
import { compose } from 'redux';
import { connect } from 'react-redux';

import injectReducer from 'utils/injectReducer';
import injectSaga from 'utils/injectSaga';

import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { PrimaryButton } from 'office-ui-fabric-react/lib/Button';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';
import TextField from 'components/TextField';
import Label from 'components/Label';

import reducer from '../VehiclePage/reducer';
import saga from './saga';
import { loadCarsEdits } from './actions';

class List extends React.Component { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);

    this._onLoadCarsEdits = this._onLoadCarsEdits.bind(this);

    this.state = {
      filter: {
        from: moment().subtract(1, 'month').format('YYYY-MM-DD'),
        to: moment().format('YYYY-MM-DD'),
      }
    };
  }

  componentDidMount() {
    this._onLoadCarsEdits();
  }

  _onLoadCarsEdits() {
    this.props.loadCarsEdits(this.state.filter);
  }

  render() {
    const { filter } = this.state;
    const { isLoading, carsEdits } = this.props;

    return (
      <div>

        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Správce autobazaru</h1>
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          <div className="row">
            <div className="col-6">
          <h3>Log úprav</h3>
          <CommandBar
            isSearchBoxVisible={ false }
            items={ [{
              key: 'from',
              onRender: () => <div className="d-flex align-items-center">
                <Label className="mx-2">Datum od</Label>
                <TextField
                  value={filter.from}
                  onChanged={(val) => this.setState({ filter: { ...filter, from: val }})}
                  placeholder='Datum od'
              /></div>,
            }, {
              key: 'to',
              onRender: () => <div className="d-flex align-items-center">
                <Label className="mx-2">Datum od</Label>
                <TextField
                  value={filter.to}
                  onChanged={(val) => this.setState({ filter: { ...filter, to: val }})}
                  placeholder='Datum do'
                /></div>,
            }, {
              key: 'submit',
              onRender: () => <div className="d-flex align-items-center">
                <Label className="mx-2"> - </Label>
                <PrimaryButton isPrimary={true} onClick={this._onLoadCarsEdits}>Filtrovat</PrimaryButton>
              </div>,
            }
            ] }
          />
          {carsEdits && carsEdits.size
            ? (
              <div>
                <table className="table table-sm">
                  <thead>
                    <tr>
                      <th>Změnil</th>
                      <th>Datum</th>
                      <th>Rok</th>
                      <th>Znacka</th>
                      <th>Cena</th>
                    </tr>
                  </thead>
                  <tbody>
                    {carsEdits.toArray().map(edit =>
                      <tr key={edit.editedBy + edit.rok + edit.validFrom}>
                        <td>
                          {edit.jmeno}<br/>
                          {edit.editedBy}
                        </td>
                        <td>{moment(edit.validFrom).format('DD.MM.YYYY')}</td>
                        <td>{edit.rok}</td>
                        <td>{edit.znacka}</td>
                        <td>{edit.cena} Kč</td>
                      </tr>
                    )}
                  </tbody>

                </table>
                {isLoading && <OpacityLoader />}
              </div>
            )
            : isLoading ? <SpinnerLoader /> : <div>No results yet</div>
          }
        </div>
      </div>
        </div>
      </div>
    );
  }
}


function mapStateToProps(state) {
  return {
    carsEdits: state.getIn(['car', 'edits']),
  }
}

function mapDispatchToProps(dispatch) {
  return {
    loadCarsEdits: (filter) => dispatch(loadCarsEdits(filter)),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);
const withSaga = injectSaga({ key: 'dashboard', saga });
const withReducer = injectReducer({ key: 'car', reducer });

export default compose(
  withConnect,
  withReducer,
  withSaga,
)(List);
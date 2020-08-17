import React from 'react';
import { compose } from 'redux';
import { connect } from 'react-redux';
import { createSelector } from 'reselect';

import injectSaga from 'utils/injectSaga';
import injectReducer from 'utils/injectReducer';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';

import Areal from './Areal';

import { loadAreas, createArea, deleteArea, updateArea, loadAreaDetails, loadAreasDistance, loadAreasUnion, resetAreas } from './actions';
import saga from './saga';
import reducer from './reducer';

class ArealPage extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);
  }

  componentDidMount() {
    this.props.loadAreas();
  }

  render() {

    const { isLoading, selectedAreaDetails, areas,
      selectedAreaDetailsIsLoading, loadAreaDetails,
      selectedAreasUnionIsLoading, selectedAreasUnion,
      selectedAreasDistanceIsLoading, selectedAreasDistance,
      onCreateArea, onUpdateArea, onDeleteArea, loadAreasDistance, loadAreasUnion, onResetAreas } = this.props;

    return (
      <div>
        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Areál</h1>
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          {areas
            ? (
              <div>
                <Areal
                  areas={areas}
                  selectedAreaDetails={selectedAreaDetails} selectedAreaDetailsIsLoading={selectedAreaDetailsIsLoading}
                  selectedAreasDistanceIsLoading={selectedAreasDistanceIsLoading} selectedAreasDistance={selectedAreasDistance}
                  selectedAreasUnionIsLoading={selectedAreasUnionIsLoading} selectedAreasUnion={selectedAreasUnion}
                  onCreateArea={onCreateArea} onUpdateArea={onUpdateArea} onDeleteArea={onDeleteArea} loadAreaDetails={loadAreaDetails}
                  loadAreasDistance={loadAreasDistance} loadAreasUnion={loadAreasUnion}
                  onResetAreas={onResetAreas}
                />
                {isLoading && <OpacityLoader />}
              </div>
            )
            : isLoading ? <SpinnerLoader /> : <div>No results yet</div>
          }
        </div>
      </div>
    );
  }
}

function mapStateToProps(state) {
  return {
    isLoading: state.getIn(['areal', 'isLoading']),
    areas: state.getIn(['areal', 'areas']),
    selectedAreaDetails: state.getIn(['areal', 'selectedAreaDetails']),
    selectedAreaDetailsIsLoading: state.getIn(['areal', 'selectedAreaDetailsIsLoading']),
    selectedAreasUnion: state.getIn(['areal', 'selectedAreasUnion']),
    selectedAreasUnionIsLoading: state.getIn(['areal', 'selectedAreasUnionIsLoading']),
    selectedAreasDistance: state.getIn(['areal', 'selectedAreasDistance']),
    selectedAreasDistanceIsLoading: state.getIn(['areal', 'selectedAreasDistanceIsLoading']),
  }
}

function mapDispatchToProps(dispatch) {
  return {
    loadAreas: () => dispatch(loadAreas()),
    loadAreaDetails: (area) => dispatch(loadAreaDetails(area)),
    loadAreasDistance: (areasIds) => dispatch(loadAreasDistance(areasIds)),
    loadAreasUnion: (areasIds) => dispatch(loadAreasUnion(areasIds)),
    onCreateArea: (area) => dispatch(createArea(area)),
    onUpdateArea: (area) => dispatch(updateArea(area)),
    onDeleteArea: (areaId) => dispatch(deleteArea(areaId)),
    onResetAreas: () => dispatch(resetAreas())
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);
const withSaga = injectSaga({ key: 'areal', saga });
const withReducer = injectReducer({ key: 'areal', reducer });

export default compose(
  withSaga,
  withConnect,
  withReducer,
)(ArealPage);
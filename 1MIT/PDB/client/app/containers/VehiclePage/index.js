import React from 'react';
import { compose } from 'redux';
import { Route } from 'react-router-dom';

import injectSaga from 'utils/injectSaga';
import injectReducer from 'utils/injectReducer';

import List from './List';
import Detail from './Detail';

import saga from './saga';
import categorySaga from '../CategoryPage/saga';
import arealSaga from '../ArealPage/saga';
import reducer from './reducer';
import categoryReducer from '../CategoryPage/reducer';
import arealReducer from '../ArealPage/reducer';

class VehiclePage extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function
  render() {
    const { match } = this.props;

    return (
      <div>
        <Route path={`${match.url}`} exact component={List}  />
        <Route path={`${match.url}/:id`} component={Detail}  />
      </div>
    );
  }
}

const withSaga = injectSaga({ key: 'car', saga });
const withCategorySaga = injectSaga({ key: 'category', saga: categorySaga });
const withArealSaga = injectSaga({ key: 'areal', saga: arealSaga });
const withReducer = injectReducer({ key: 'car', reducer });
const withCategoryReducer = injectReducer({ key: 'category', reducer: categoryReducer });
const withArealReducer = injectReducer({ key: 'areal', reducer: arealReducer });

export default compose(
  withSaga,
  withCategorySaga,
  withArealSaga,
  withReducer,
  withCategoryReducer,
  withArealReducer,
)(VehiclePage);
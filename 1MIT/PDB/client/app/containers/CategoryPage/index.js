import React from 'react';
import { compose } from 'redux';
import { connect } from 'react-redux';
import { createSelector } from 'reselect';

import injectSaga from 'utils/injectSaga';
import injectReducer from 'utils/injectReducer';

import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

import SectionHeader from 'components/SectionHeader';
import { SpinnerLoader, OpacityLoader } from 'components/Loader';

import Table from './Table';
import AddPanel from './AddPanel';

import { loadCategories, createCategory, updateCategory, deleteCategory } from './actions';
import saga from './saga';
import reducer from './reducer';

class CategoryPage extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);

    this._onShowPanel = this._onShowPanel.bind(this);
    this._onClosePanel = this._onClosePanel.bind(this);
    this._onCreateCategory = this._onCreateCategory.bind(this);

    this.state = {
      showPanel: false,
    };
  }

  componentDidMount() {
    this.props.loadCategories();
  }

  _onCreateCategory(data) {
    this.props.onCreateCategory(data);
    this._onClosePanel();
  }

  _onShowPanel() {
    this.setState({ showPanel: true });
  }

  _onClosePanel() {
    this.setState({ showPanel: false });
  }

  render() {

    const { categories, isLoading, onUpdateCategory, onDeleteCategory } = this.props;

    return (
      <div>
        <SectionHeader>
          <div className="row align-items-end">
            <div className="col">
              <h1>Kategorie vozidel</h1>
            </div>
            <div className="col-auto">
              <DefaultButton
                primary={ true }
                onClick={this._onShowPanel}
              >
                Přidat novou
              </DefaultButton>
            </div>
          </div>
        </SectionHeader>
        <div style={{padding: "0 2rem"}}>
          {categories && categories.size
            ? (
              <div>
                <Table items={categories} onDeleteCategory={onDeleteCategory} onUpdateCategory={onUpdateCategory} />
                {isLoading && <OpacityLoader />}
              </div>
            )
            : isLoading ? <SpinnerLoader /> : <div>No results yet</div>
          }
        </div>
        <AddPanel
          header="Přidat kategorii"
          form={{}}
          onClosePanel={this._onClosePanel}
          onSubmit={this._onCreateCategory}
          show={this.state.showPanel}/>
      </div>
    );
  }
}

function mapStateToProps(state) {
  return {
    categories: state.getIn(['category', 'categories']),
    isLoading: state.getIn(['category', 'isLoading'])
  }
}

function mapDispatchToProps(dispatch) {
  return {
    loadCategories: () => dispatch(loadCategories()),
    onCreateCategory: (category) => dispatch(createCategory(category)),
    onUpdateCategory: (category) => dispatch(updateCategory(category)),
    onDeleteCategory: (category) => dispatch(deleteCategory(category.id)),
  };
}

const withConnect = connect(mapStateToProps, mapDispatchToProps);
const withSaga = injectSaga({ key: 'category', saga });
const withReducer = injectReducer({ key: 'category', reducer });

export default compose(
  withSaga,
  withConnect,
  withReducer,
)(CategoryPage);
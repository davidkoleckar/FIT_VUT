import React from 'react';
import moment from 'moment';
import { Link } from 'react-router-dom';

import { CheckboxVisibility } from 'office-ui-fabric-react/lib/DetailsList';
import { ActionButton } from 'office-ui-fabric-react/lib/Button';

import Table from 'components/Table';
import Img from 'components/Img';

const getColumnsConfig = (props) => [
  {
    name: "Fotka",
    onRender: item => <Img src={ item.obrazekData ? `data:image/jpeg;base64,${item.obrazekData}` : "https://placebear.com/100/100" } />,
    maxWidth: 60,
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
    minWidth: 60,
    onRender: item => <div>
      <ActionButton
        iconProps={ { iconName: 'Car' } }
      >
        <Link to={`/vozidla/${item.id}`}>Detail</Link>
      </ActionButton>
    </div>,
  },

];

class CarsTable extends React.PureComponent {

  constructor(props) {
    super(props);
  }

  render() {
    const { items } = this.props;

    return (
      <div>
        <Table
            items={ items.toJS() }
            checkboxVisibility={CheckboxVisibility.hidden}
            columns={ getColumnsConfig(this.props) }
        />
      </div>
    );
  }
}

export default CarsTable;
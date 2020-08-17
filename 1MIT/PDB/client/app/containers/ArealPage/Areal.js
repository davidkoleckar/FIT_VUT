import React from 'react';
import {Layer, Rect, Group, Line, Circle, Stage } from 'react-konva';

import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';

import { StyledStage } from 'components/Areal/Editor';
import { SpinnerLoader } from 'components/Loader';
import TextField from 'components/TextField';
import SoldToggle from 'components/Vehicle/SoldToggle';

import Table from './Table';

class Areal extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function


  constructor(props) {
    super(props);
    this.handleClick = this.handleClick.bind(this);
    this._updateForm = this._updateForm.bind(this);
    this._submitForm = this._submitForm.bind(this);
    this._onDeleteArea = this._onDeleteArea.bind(this);
    this._updateDragOffset = this._updateDragOffset.bind(this);
    this._updateShapeRadius = this._updateShapeRadius.bind(this);
    this._selectShape = this._selectShape.bind(this);
    this._handleShape = this._handleShape.bind(this);
    this._renderSoldSwitch = this._renderSoldSwitch.bind(this);
    this._renderUnionSwitch = this._renderUnionSwitch.bind(this);
    this._renderDistanceSwitch = this._renderDistanceSwitch.bind(this);
    this._getStrokeColor = this._getStrokeColor.bind(this);

    this.state = {
      newShape: null,
      selectedShape: null,
      modeDraw: false,
      modeDistance: false,
      modeUnion: false,
    }
  }


  _updateForm(update) {
    this.setState({
      selectedShape: { ...this.state.selectedShape, ...update }
    });
  }

  _submitForm() {
    if (this.state.selectedShape.id) {
      this.props.onUpdateArea(this.state.selectedShape);
    } else {
      this.props.onCreateArea(this.state.selectedShape);
    }
    this.setState({
      selectedShape: null,
      newShape: null,
      modeDraw: false,
      modeDistance: false,
      modeUnion: false,
    });
  }

  _onDeleteArea() {
    if (this.state.selectedShape.id) {
      this.props.onDeleteArea(this.state.selectedShape.id);
    }
  }

  _updateDragOffset(e) {
    const attrs = e.target.getAttrs();
    const offset = { offsetX: attrs.x, offsetY: attrs.y };
    this.setState({
      selectedShape: { ...this.state.selectedShape, ...offset}
    });
  }

  _updateShapeRadius(val) {
    const newShape = this.state.selectedShape.tvar.splice(0);
    newShape[1].x = val * 1;

    this.setState({
      selectedShape: {...this.state.selectedShape, tvar: newShape }
    });
  }

  _renderSoldSwitch() {
    return (
      <SoldToggle
        checked={ this.state.modeDraw }
        onChanged={() => this.setState({
          modeDraw: !this.state.modeDraw,
          modeDistance: false,
          modeUnion: false,
          selectedShape: {
            tvar: [],
            typEntity: 'ctverec',
          },
        })}
        label='Kreslící režim'
        offText='Off'
        onText='On'
      />
    );
  }

  _renderUnionSwitch() {
    return (
      <SoldToggle
        checked={ this.state.modeUnion }
        onChanged={() => {
          this.props.onResetAreas();
          this.setState({
            modeDraw: false,
            modeDistance: false,
            modeUnion: !this.state.modeUnion,
            unionIds: [],
          })
        }}
        label='Vypsat vozidla'
        offText='Off'
        onText='On'
      />
    );
  }

  _renderDistanceSwitch() {
    return (
      <SoldToggle
        checked={ this.state.modeDistance }
        onChanged={() => {
          this.props.onResetAreas();
          this.setState({
              modeDraw: false,
              modeDistance: !this.state.modeDistance,
              modeUnion: false,
              distanceIds: [],
            })
          }
        }
        label='Výpočet vzdálenosti'
        offText='Off'
        onText='On'
      />
    );
  }

  handleClick(e) {
    const { selectedShape, modeDraw } = this.state;
    if (!modeDraw) return;
    const position = this.stage.getStage().getPointerPosition();
    const newShape = (selectedShape.tvar || []).slice(0);
    newShape.pop();
    newShape.push({x: position.x, y: position.y});
    newShape.push(newShape[0]);

    this.setState({
      selectedShape: { ...selectedShape, tvar: newShape},
    })
  }

  normalizedPoints(area) {
    return area.tvar.reduce((acc, point) => {
      acc.push(point.x);
      acc.push(point.y);
      return acc;
    }, []);
  };

  getFillColor(typ) {
    switch(typ) {
      case 'parkoviste':
        return '#eaeaea';
      case 'budova':
        return '#008272';
      case 'cesta':
        return '#ffb900';
      case 'sekce':
        return '#fff100';
      default:
        return '#000';
    }
  }

  _getStrokeColor(areaId) {
    if (this.state.modeDistance && this.state.distanceIds.indexOf(areaId) > -1) {
      return '#00B294';
    } else if (this.state.modeUnion && this.state.unionIds.indexOf(areaId) > -1) {
      return '#e3008c';
    } else if (this.state.selectedShape && this.state.selectedShape.id === areaId) {
        return '#0078d7';
    }
    return '#a6a6a6';
  }

  _selectShape(areaId) {
    if (this.state.modeUnion && this.state.unionIds.length <= 2) {
      if (this.state.unionIds.indexOf(areaId) === -1) {
        const unionIds = this.state.unionIds.splice(0);
        unionIds.push(areaId);
        if (unionIds.length === 2) {
          // load union
          this.props.loadAreasUnion(unionIds);
        }
        this.setState({
          unionIds
        })
      }
    } else if (this.state.modeDistance && this.state.distanceIds.length <= 2) {
      if (this.state.distanceIds.indexOf(areaId) === -1) {
        const distanceIds = this.state.distanceIds.splice(0);
        distanceIds.push(areaId);
        if (distanceIds.length === 2) {
          // load distance
          this.props.loadAreasDistance(distanceIds);
        }
        this.setState({
          distanceIds
        });
      }
    } else {
      if (!this.state.selectedShape || this.state.selectedShape.id !== areaId) {
        let selectedShape = this.props.areas.get(areaId);
        this.props.loadAreaDetails(selectedShape);
        this.setState({
          selectedShape
        });
      }
    }
  }

  _handleShape(area) {
    const { selectedShape } = this.state;

    if (area.typEntity === 'kruh') {
      return (<Circle
          x={selectedShape && selectedShape.offsetX && selectedShape.id === area.id ? selectedShape.offsetX : area.tvar[0].x}
          y={selectedShape && selectedShape.offsetY && selectedShape.id === area.id ? selectedShape.offsetY : area.tvar[0].y}
          radius={selectedShape && selectedShape.offsetY && selectedShape.id === area.id ? selectedShape.tvar[1].x :  area.tvar[1].x}
          key={`${area.id}-${area.edited ? area.edited : ''}`}
          fill={this.getFillColor(area.typ)}
          onClick={() => this._selectShape(area.id)}
          stroke={this._getStrokeColor(area.id)}
          strokeWidth={ 3 }
          draggable={selectedShape && selectedShape.id === area.id}
          onDragEnd={this._updateDragOffset}
        />
      );
    }

    return (
      <Line
        key={`${area.id}-${area.edited ? area.edited : ''}`}
        points={ this.normalizedPoints(area) }
        offsetX={0}
        offsetY={0}
        fill={this.getFillColor(area.typ)}
        onClick={() => this._selectShape(area.id)}
        stroke={this._getStrokeColor(area.id)}
        strokeWidth={ 3 }
        draggable={selectedShape && selectedShape.id === area.id}
        onDragEnd={this._updateDragOffset}
        closed
      />
    )
  }

  render() {
    const {
      areas, selectedAreaDetails, selectedAreaDetailsIsLoading,
      selectedAreasUnionIsLoading, selectedAreasUnion,
      selectedAreasDistanceIsLoading, selectedAreasDistance
    } = this.props;
    const { selectedShape, modeUnion, modeDistance } = this.state;

    const areaDetails = selectedAreaDetails ? selectedAreaDetails.toJS() : null;

    return (
      <div>
        <CommandBar
          isSearchBoxVisible={ false }
          items={ [
            {
              key: 'sold',
              onRender: this._renderSoldSwitch,
              name: 'Prodane',
            },{
              key: 'distance',
              onRender: this._renderDistanceSwitch,
              name: 'Vzdalenost',
            },{
              key: 'union',
              onRender: this._renderUnionSwitch,
              name: 'Spojeni',
            },{
              key: 'addSlot',
              name: 'Přidat místo',
              icon: 'Add',
              onClick: () => this.setState({
                selectedShape: {
                  nazev: "Nový čtverec",
                  tvar: [{x:50,y:50},{x:100,y:50},{x:100,y:100},{x:50,y:100},{x:50,y:50}],
                  typEntity: 'ctverec',
                  typ: 'parkoviste'
                }
              })
            },{
              key: 'addCircle',
              name: 'Přidat kruh',
              icon: 'CircleFill',
              onClick: () => this.setState({
                selectedShape: {
                  nazev: "Nový kruh",
                  tvar: [{x:150, y:150}, {x:100}],
                  typEntity: 'kruh',
                  typ: 'budova'
                }
              })
            },
          ] }
        />
        <div className="row">
          <div className="col-auto">
        <StyledStage>
          <Stage
            width={700}
            height={500}
            onContentClick={this.handleClick}
            ref={(elm) => { this.stage = elm}}
          >
            <Layer>
              {areas.toArray().map(this._handleShape)}
              {areaDetails && areaDetails.parkingSpots && areaDetails.parkingSpots.map(this._handleShape)}
              {selectedShape && !selectedShape.id && this._handleShape(selectedShape)}
            </Layer>
          </Stage>
        </StyledStage>
          </div>
          {(selectedAreasDistance || selectedAreasDistanceIsLoading) && !modeUnion && modeDistance && <div className="col-4">
            <h3>Vzdalenost sekcí</h3>
            {selectedAreasDistanceIsLoading ? <SpinnerLoader /> : <strong>{selectedAreasDistance} m</strong>}
          </div>}

          {(selectedAreasUnion || selectedAreasUnionIsLoading) && modeUnion && !modeDistance && <div className="col">
            <h3>Výpis aut v sekcích</h3>
            {selectedAreasUnionIsLoading ? <SpinnerLoader /> : <Table items={selectedAreasUnion} />}
          </div>}


          {!modeUnion && !modeDistance && selectedShape && <div className="col">
            <h3>Vybrany tvar</h3>

            <div className="row mb-3">
              <div className="col-3">Název entity</div>
              <div className="col-6">
                <TextField
                  placeholder='Nazev'
                  value={selectedShape.nazev}
                  onChanged={(val) => this._updateForm({nazev: val})}
                  required
                />
              </div>
            </div>

            {selectedShape.typEntity === 'kruh' &&
            <div className="row mb-3">
              <div className="col-3">Polomer</div>
              <div className="col-6">
                <TextField
                  placeholder='Nazev'
                  value={selectedShape.tvar[1].x}
                  onChanged={this._updateShapeRadius}
                  required
                />
              </div>
            </div>
            }

            <div className="row mb-3">
              <div className="col-3">Typ</div>
              <div className="col-6">
                <Dropdown
                  placeHolder='Vyberte'
                  selectedKey={selectedShape.typ}
                  options={[{key: 'areal', text: 'areal'},{key: 'sekce', text: 'sekce'},{key: 'parkoviste', text: 'parkoviště'}, {key: 'budova', text: 'budova'}, {key: 'cesta', text: 'cesta'}]}
                  onChanged={(item) => this._updateForm({typ: item.key})}
                  required
                />
              </div>
            </div>

            <div className="row mb-3">
              <div className="col-9">
                <div className="d-flex justify-content-end">
                  <DefaultButton className="mr-2" onClick={this._onDeleteArea}>Smazat</DefaultButton>
                  <PrimaryButton onClick={this._submitForm}>Uložit</PrimaryButton>
                </div>
              </div>
            </div>

            {areaDetails && <div>
            <h3>Podrobnosti o místě</h3>

            <div className="row mb-3">
              <div className="col-9">
                {selectedAreaDetailsIsLoading ? <SpinnerLoader/> : <table className="table table-sm">
                  <thead>
                  <tr>
                    <th>Údaj</th>
                    <th>Hodnota</th>
                  </tr>
                  </thead>
                  <tbody>
                    <tr><td>Obvod</td><td>{Math.floor(areaDetails.length)} m</td></tr>
                    <tr><td>Plocha</td><td>{Math.floor(areaDetails.space)} m<sup>2</sup></td></tr>
                    {selectedShape.typ === 'sekce' && <tr><td>Průměrná cena vozu</td><td>{Math.floor(areaDetails.averagePrice)} Kč</td></tr>}
                    {selectedShape.typ === 'sekce' && <tr><td>Pocet míst</td><td>{areaDetails.parkingSpots.length || 0}</td></tr>}
                  </tbody>

                </table>
                }
              </div>
            </div>
            </div>}
          </div>}
        </div>
      </div>
    );
  }
}

export default Areal;

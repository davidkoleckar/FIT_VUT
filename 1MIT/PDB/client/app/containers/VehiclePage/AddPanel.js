import React from 'react';

import { PanelType } from 'office-ui-fabric-react/lib/Panel';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';

import Panel from 'components/Panel';
import TextField from 'components/TextField';

class AddPanel extends React.Component { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);
    this._onRenderFooterContent = this._onRenderFooterContent.bind(this);
    this._updateForm = this._updateForm.bind(this);
    this._submitForm = this._submitForm.bind(this);

    this.state = {
      form: props.form,
    }
  }

  _updateForm(update) {
    this.setState({
      form: { ...this.state.form, ...update }
    });
  }

  _submitForm() {
    if (!this.state.form.umisteni) return;
    this.props.onSubmit(this.state.form);
  }

  _onRenderFooterContent() {
    return (
      <div>
        <PrimaryButton
          onClick={this._submitForm}
          className="mr-2"
        >
          Přidat
        </PrimaryButton>
        <DefaultButton
          onClick={ this.props.onClosePanel }
        >
          Zrušit
        </DefaultButton>
      </div>
    );
  }

  _onRenderBody() {
    const { categories, emptyAreas } = this.props;
    const { form: { umisteni, znacka, model, rok, stk, motor, kupniCena, cena, kilometry, stav, kategorie } } = this.state;

    const umisteniOptions = emptyAreas.toArray().map(cat => ({key: cat, text: cat}) );
    if (umisteni) {
      umisteniOptions.push({key: umisteni, text: umisteni});
    }
    
    return (
      <div>
        <div className="row mb-3">
          <div className="col-3">Pozice</div>
          <div className="col-8">
            {emptyAreas && <Dropdown
              placeHolder='Vyberte'
              selectedKey={umisteni}
              options={umisteniOptions}
              onChanged={(item) => this._updateForm({umisteni: item.key})}
              required
            />}
          </div>
        </div>
        <div className="row mb-3">
          <div className="col-3">Kategorie</div>
          <div className="col-8">
            {categories && <Dropdown
              placeHolder='Vyberte'
              selectedKey={kategorie}
              options={categories.toArray().map(cat => ({key: cat.id, text: cat.nazev}))}
              onChanged={(item) => this._updateForm({kategorie: item.key})}
              required
            />}
          </div>
        </div>

        <div className="row mb-3">
          <div className="col-3">Rok vyroby</div>
          <div className="col-3">
            <TextField
              placeholder='2012'
              value={rok}
              onChanged={(val) => this._updateForm({rok: val * 1})}
              required
            />
          </div>
          <div className="col-2">STK</div>
          <div className="col-3">
            <TextField
              placeholder='08/2016'
              value={stk}
              onChanged={(val) => this._updateForm({stk: val})}
            />
          </div>
        </div>


        <div className="row mb-3">
          <div className="col-3">Kupni cena</div>
          <div className="col-3">
            <TextField
              placeholder='300,000'
              value={kupniCena}
              onChanged={(val) => this._updateForm({kupniCena: val * 1})}
              required
            />
          </div>
          <div className="col-2">Cena</div>
          <div className="col-3">
            <TextField
              placeholder='350,000'
              value={cena}
              onChanged={(val) => this._updateForm({cena: val * 1})}
              required
            />
          </div>
        </div>

        <div className="row mb-3">
          <div className="col-3">Znacka</div>
          <div className="col-3">
            <TextField
              placeholder='BMW, Mercedes, Audi, ...'
              value={znacka}
              onChanged={(val) => this._updateForm({znacka: val})}
              required
            />
          </div>
          <div className="col-2">Model</div>
          <div className="col-3">
            <TextField
              placeholder='330i, A3, ...'
              value={model}
              onChanged={(val) => this._updateForm({model: val})}
            />
          </div>
        </div>

        <div className="row mb-3">
          <div className="col-3">Motor</div>
          <div className="col-8">
            <TextField
              placeholder='3.0 TDI'
              value={motor}
              onChanged={(val) => this._updateForm({motor: val})}
              required
            />
          </div>
        </div>

        <div className="row mb-3">
          <div className="col-3">Stav tachometru</div>
          <div className="col-8">
            <TextField
              placeholder='152,340'
              value={kilometry}
              onChanged={(val) => this._updateForm({kilometry: val * 1})}
              required
            />
          </div>
        </div>

        <div className="row mb-3">
          <div className="col-3">Stav</div>
          <div className="col">
            <TextField
              placeholder='Dobrý technický stav, jen volant nefunguje...'
              value={stav || ''}
              onChanged={(val) => this._updateForm({stav: val})}
              multiline
            />
          </div>
        </div>
      </div>
    );
  }

  render() {
    const { show, onClosePanel, header } = this.props;
    return (

      <Panel
        isOpen={ show }
        type={ PanelType.medium }
        onDismiss={ onClosePanel }
        headerText={header}
        closeButtonAriaLabel='Close'
        onRenderFooterContent={ this._onRenderFooterContent }
      >
        {this._onRenderBody()}
      </Panel>
    );
  }
}

export default AddPanel;

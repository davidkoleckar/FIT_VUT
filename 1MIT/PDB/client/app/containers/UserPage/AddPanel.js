import React from 'react';
import { PanelType } from 'office-ui-fabric-react/lib/Panel';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';

import Panel from 'components/Panel';
import TextField from 'components/TextField';

export default class AddPanel extends React.Component { // eslint-disable-line react/prefer-stateless-function

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

    render() {

      const { show, header, onClosePanel } = this.props;
      const { form: { jmeno, email, heslo, adresa } } = this.state;
        return (
          <Panel
              isOpen={ show }
              type={ PanelType.medium }
              onClick={ onClosePanel }
              headerText={header}
              closeButtonAriaLabel='Zavřít'
              onRenderFooterContent={ this._onRenderFooterContent }
          >
            <form onSubmit={(e) => { e.preventDefault(); this._submitForm() }}>
            <div className="row mb-3">
              <div className="col-3">Jméno</div>
              <div className="col">
                <TextField
                  placeholder='Honza Test'
                  value={jmeno}
                  onChanged={(val) => this._updateForm({jmeno: val})}
                  required
                />
              </div>
            </div>
            <div className="row mb-3">
              <div className="col-3">Email</div>
              <div className="col">
                <TextField
                  placeholder='aaa@bbb.cz'
                  value={email}
                  onChanged={(val) => this._updateForm({email: val})}
                  required
                />
              </div>
            </div>
            <div className="row mb-3">
              <div className="col-3">Heslo</div>
              <div className="col">
                <TextField
                  placeholder='TajneHeslo'
                  type='password'
                  value={heslo}
                  onChanged={(val) => this._updateForm({heslo: val})}
                  required
                />
              </div>
            </div>
            <div className="row mb-3">
              <div className="col-3">Adresa</div>
              <div className="col">
                <TextField
                  multiline
                  value={adresa}
                  onChanged={(val) => this._updateForm({adresa: val})}
                />
              </div>
            </div>
            </form>
          </Panel>
        );
    }
}

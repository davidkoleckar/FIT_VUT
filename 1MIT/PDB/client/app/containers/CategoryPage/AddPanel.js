import React from 'react';

import { PanelType } from 'office-ui-fabric-react/lib/Panel';
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
    const { form: { nazev } } = this.state;
     return (
          <Panel
              isOpen={ show }
              type={ PanelType.medium }
              onDismiss={ onClosePanel }
              headerText={ header }
              onRenderFooterContent={ this._onRenderFooterContent }
          >
            <div className="row mb-3">
                <div className="col-3">Název</div>
                <div className="col">
                  <TextField
                    placeholder='Karavany'
                    value={nazev}
                    onChanged={(val) => this._updateForm({ nazev: val })}
                    required
                  />
                </div>
            </div>
          </Panel>
        );
    }
}

export default AddPanel;

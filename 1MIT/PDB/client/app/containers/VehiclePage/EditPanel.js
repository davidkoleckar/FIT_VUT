import React from 'react';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';

import { PanelType } from 'office-ui-fabric-react/lib/Panel';

import Panel from 'components/Panel';
import TextField from 'components/TextField';

import AddPanel from './AddPanel';

export default class EditPanel extends AddPanel { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);
    this._parseFile = this._parseFile.bind(this);
    this._submitForm = this._submitForm.bind(this);
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

  _submitForm() {
    if (!this.state.form.umisteni) return;
    this.props.onSubmit(this.state.form);
    if (this.state.photo) {
      this.props.onUploadPhoto(this.state.form, this.state.photo);
    }
  }

  _onRenderFooterContent() {
    return (
      <div>
        <PrimaryButton
          onClick={this._submitForm}
          className="mr-2"
        >
          Uložit
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

        <div className="row mb-3">
          <div className="col-3">Fotka</div>
          <div className="col-8">
            <input
              type="file"
              onChange={this._parseFile}
            />
          </div>
        </div>
        {this._onRenderBody()}
      </Panel>
    );
  }
}

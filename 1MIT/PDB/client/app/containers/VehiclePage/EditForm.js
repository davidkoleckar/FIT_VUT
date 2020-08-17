import React from 'react';

import { PrimaryButton } from 'office-ui-fabric-react/lib/Button';
import AddPanel from './AddPanel';

export default class EditForm extends AddPanel { // eslint-disable-line react/prefer-stateless-function

  constructor(props) {
    super(props);
    this._submitForm = this._submitForm.bind(this);
  }

  _submitForm() {
    if (!this.state.form.umisteni) return;
    this.props.onSubmit(this.state.form);
  }

  render() {
    return (
      <div>
        {this._onRenderBody()}
        <div className="row justify-content-end">
          <div className="col-auto">
            <PrimaryButton onClick={this._submitForm}>Uložit změny</PrimaryButton>
          </div>
        </div>
      </div>
    );
  }
}

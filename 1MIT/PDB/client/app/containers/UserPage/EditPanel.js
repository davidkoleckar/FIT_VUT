import React from 'react';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';

import AddPanel from './AddPanel';

export default class EditPanel extends AddPanel { // eslint-disable-line react/prefer-stateless-function

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
}

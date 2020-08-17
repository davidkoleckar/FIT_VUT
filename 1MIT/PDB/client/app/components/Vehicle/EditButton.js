import React from 'react';

import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

const EditButton = () => (
  <DefaultButton
    primary
    text="Upravit"
    onClick={ () => {} }
    split={ true }
    menuProps={ {
      items: [
        {
          key: 'detail',
          name: 'Detail',
          icon: 'Mail',
        },
        {
          key: 'record',
          name: 'Upravit',
          icon: 'PlanView',
        },
        {
          key: 'delete',
          name: 'Smazat',
          iconProps: {
            iconName: 'Delete',
            style: {
              color: 'red'
            }
          },
        }
      ]
    } }
  />
);

export default EditButton;
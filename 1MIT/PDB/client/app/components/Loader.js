import React from 'react';
import styled from 'styled-components';
import { Spinner, SpinnerSize } from 'office-ui-fabric-react/lib/Spinner';

const OpacityLoader = styled.div`
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    background-color: rgba(255,255,255,0.3);
`;

const SpinnerLoader = () => <div className="p-5">
  <Spinner size={ SpinnerSize.large } />
</div>;


export {
  OpacityLoader,
  SpinnerLoader,
}